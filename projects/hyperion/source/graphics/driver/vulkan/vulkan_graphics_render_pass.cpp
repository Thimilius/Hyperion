//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_render_pass.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsRenderPass::VulkanGraphicsRenderPass(VulkanGraphicsDevice *device, const GraphicsRenderPassDescription &description) : GraphicsRenderPass(device, description) {
        VkAttachmentDescription color_attachment = { };
        color_attachment.format = device->GetContext()->GetSwapChain()->GetFormat();
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference color_attachment_reference = { };
        color_attachment_reference.attachment = 0;
        color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        
        VkSubpassDescription subpass = { };
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_reference;

        VkRenderPassCreateInfo render_pass_create_info = { };
        render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount = 1;
        render_pass_create_info.pAttachments = &color_attachment;
        render_pass_create_info.subpassCount = 1;
        render_pass_create_info.pSubpasses = &subpass;

        HYP_VULKAN_CHECK(vkCreateRenderPass(device->GetDevice(), &render_pass_create_info, nullptr, &m_render_pass));
    }

    //--------------------------------------------------------------
    VulkanGraphicsRenderPass::~VulkanGraphicsRenderPass() {
        VulkanGraphicsDevice *device = static_cast<VulkanGraphicsDevice *>(GetDevice());
        vkDestroyRenderPass(device->GetDevice(), m_render_pass, nullptr);
    }

}