//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"
#include "hyperion/render/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void VulkanRenderDriver::Initialize() { }

  //--------------------------------------------------------------
  void VulkanRenderDriver::HandleAssets(RenderAssetContext &asset_context) { }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Render(RenderFrame *render_frame) {
    VkDevice device = m_context->m_device;

    vkWaitForFences(device, 1, &m_context->m_in_flight_fences[m_current_frame_index], VK_TRUE, UINT64_MAX);

    if (RenderEngine::RenderShouldResize()) {
      m_context->RecreateSwapchain();
    }

    uint32 image_index = 0;
    VkResult present_result = vkAcquireNextImageKHR(device, m_context->m_swapchain, UINT64_MAX, m_context->m_image_available_semaphores[m_current_frame_index],
                                                    VK_NULL_HANDLE, &image_index);
    if (present_result == VK_ERROR_OUT_OF_DATE_KHR) {
      m_context->RecreateSwapchain();
    } else if (present_result != VK_SUCCESS && present_result != VK_SUBOPTIMAL_KHR) {
      HYP_PANIC_MESSAGE("Vulkan", "Failed to acquire swapchain image!");
    }

    UpdateUniformBuffer(m_current_frame_index);

    // Only reset the fence if we are submitting work.
    vkResetFences(device, 1, &m_context->m_in_flight_fences[m_current_frame_index]);

    vkResetCommandBuffer(m_context->m_command_buffers[m_current_frame_index], 0);
    RecordCommandBuffer(m_context->m_command_buffers[m_current_frame_index], image_index, m_current_frame_index);

    VkSubmitInfo submit_info = { };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore wait_semaphores[] = { m_context->m_image_available_semaphores[m_current_frame_index] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_context->m_command_buffers[m_current_frame_index];
    VkSemaphore signal_semaphores[] = { m_context->m_render_finished_semaphores[m_current_frame_index] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    HYP_VULKAN_CHECK(
      vkQueueSubmit(m_context->m_graphics_queue, 1, &submit_info, m_context->m_in_flight_fences[m_current_frame_index]),
      "Failed to submit command buffer!"
    );

    VkPresentInfoKHR present_info = { };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    VkSwapchainKHR swapchains[] = { m_context->m_swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr;

    present_result = vkQueuePresentKHR(m_context->m_presentation_queue, &present_info);
    if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR) {
      m_context->RecreateSwapchain();
    }

    m_current_frame_index = (m_current_frame_index + 1) % VulkanRenderContext::MAX_FRAMES_IN_FLIGHT;
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Shutdown() {
    vkDeviceWaitIdle(m_context->m_device);
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Setup(VulkanRenderContext *context) {
    m_context = context;
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::RecordCommandBuffer(VkCommandBuffer command_buffer, uint32 image_index, uint32 frame_index) {
    VkCommandBufferBeginInfo begin_info = { };
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    HYP_VULKAN_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info), "Failed to begin command buffer!");

    VkViewport viewport = { };
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float32>(Display::GetWidth());
    viewport.height = static_cast<float32>(Display::GetHeight());
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &viewport);
    VkRect2D scissor = { };
    scissor.offset = { 0, 0 };
    scissor.extent = { Display::GetWidth(), Display::GetHeight() };
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);
    
    VkRenderPassBeginInfo render_pass_begin_info = { };
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = m_context->m_render_pass;
    render_pass_begin_info.framebuffer = m_context->m_swapchain_framebuffers[image_index];
    render_pass_begin_info.renderArea.offset = { 0, 0 };
    render_pass_begin_info.renderArea.extent = m_context->m_swapchain_extent;
    VkClearValue clear_color = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;
    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_context->m_graphics_pipeline);
    VkDeviceSize buffer_offsets = 0;
    vkCmdBindVertexBuffers(command_buffer, 0, 1, &m_context->m_vertex_buffer, &buffer_offsets);
    vkCmdBindIndexBuffer(command_buffer, m_context->m_index_buffer, 0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(
      command_buffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      m_context->m_pipeline_layout,
      0,
      1,
      &m_context->m_descriptor_sets[frame_index],
      0,
      nullptr
    );
    VulkanPushConstant push_constant;
    push_constant.model = Matrix4x4::Rotate(Vector3(0.0f, 0.0f, 1.0f), Time::GetTime() * 50.0f); 
    vkCmdPushConstants(command_buffer, m_context->m_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push_constant), &push_constant);
    vkCmdDrawIndexed(command_buffer, 6, 1, 0, 0, 0);
    push_constant.model = Matrix4x4::Translate(0.0f, 0.0f, -1.0f) * Matrix4x4::Rotate(Vector3(0.0f, 0.0f, 1.0f), Time::GetTime() * 25.0f); 
    vkCmdPushConstants(command_buffer, m_context->m_pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(push_constant), &push_constant);
    vkCmdDrawIndexed(command_buffer, 6, 1, 0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    HYP_VULKAN_CHECK(vkEndCommandBuffer(command_buffer), "Failed to record command buffer!");
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::UpdateUniformBuffer(uint32 frame_index) {
    VulkanUniformBufferObject ubo;
    ubo.view = Matrix4x4::LookAt(Vector3(2.0f, 2.0f, 2.0f), Vector3::Zero(), Vector3(0.0f, 1.0f, 0.0f));
    float32 aspect_ratio = static_cast<float32>(m_context->m_swapchain_extent.width) / static_cast<float32>(m_context->m_swapchain_extent.height);
    ubo.projection = Matrix4x4::Perspective(45.0f, aspect_ratio, 0.1f, 10.0f);
    ubo.projection.elements[5] *= -1;

    void *data;
    vmaMapMemory(m_context->m_allocator, m_context->m_uniform_buffer_memories[frame_index], &data);
    std::memcpy(data, &ubo, sizeof(ubo));
    vmaUnmapMemory(m_context->m_allocator, m_context->m_uniform_buffer_memories[frame_index]);
  }

}
