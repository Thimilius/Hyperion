//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void VulkanRenderDriver::Initialize() {

  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::HandleAssets(RenderAssetContext &asset_context) {
    
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Render(RenderFrame *render_frame) {
    VkDevice device = m_context->m_device;

    vkWaitForFences(device, 1, &m_context->m_in_flight_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &m_context->m_in_flight_fence);

    uint32 image_index = 0;
    vkAcquireNextImageKHR(device, m_context->m_swapchain, UINT64_MAX, m_context->m_image_available_semaphore, VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(m_context->m_command_buffer, 0);
    RecordCommandBuffer(m_context->m_command_buffer, image_index);

    VkSubmitInfo submit_info = { };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore wait_semaphores[] = { m_context->m_image_available_semaphore };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &m_context->m_command_buffer;
    VkSemaphore signal_semaphores[] = { m_context->m_render_finished_semaphore };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;
    
    HYP_VULKAN_CHECK(vkQueueSubmit(m_context->m_graphics_queue, 1, &submit_info, m_context->m_in_flight_fence), "Failed to submit command buffer!");

    VkPresentInfoKHR present_info = { };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    VkSwapchainKHR swapchains[] = { m_context->m_swapchain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swapchains;
    present_info.pImageIndices = &image_index;
    present_info.pResults = nullptr;

    vkQueuePresentKHR(m_context->m_presentation_queue, &present_info);
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
  void VulkanRenderDriver::RecordCommandBuffer(VkCommandBuffer command_buffer, uint32 image_index) {
    VkCommandBufferBeginInfo begin_info = { };
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    HYP_VULKAN_CHECK(vkBeginCommandBuffer(command_buffer, &begin_info), "Failed to begin command buffer!");

    VkRenderPassBeginInfo render_pass_begin_info = { };
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = m_context->m_render_pass;
    render_pass_begin_info.framebuffer = m_context->m_swapchain_framebuffers[image_index];
    render_pass_begin_info.renderArea.offset = { 0, 0 };
    render_pass_begin_info.renderArea.extent = m_context->m_swapchain_extent;

    VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_context->m_graphics_pipeline);
    vkCmdDraw(command_buffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    HYP_VULKAN_CHECK(vkEndCommandBuffer(command_buffer), "Failed to record command buffer!");
  }

}