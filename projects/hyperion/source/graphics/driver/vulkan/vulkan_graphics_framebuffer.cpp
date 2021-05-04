//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_framebuffer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_render_pass.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_swap_chain.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsFramebuffer::VulkanGraphicsFramebuffer(VulkanGraphicsDevice *device, const GraphicsFramebufferDescription &description) : GraphicsFramebuffer(device, description) {
        uint32 attachment_count = static_cast<uint32>(description.attachments.size());

        Vector<VkImageView> attachments(attachment_count);
        for (GraphicsTextureView *texture_view : description.attachments) {
            // TODO: Implement.
        }
        VkExtent2D swap_chain_extent = device->GetContext()->GetSwapChain()->GetExtent();

        VkFramebufferCreateInfo framebuffer_create_info = { };
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.renderPass = reinterpret_cast<VulkanGraphicsRenderPass *>(description.render_pass)->GetRenderPass();
        framebuffer_create_info.attachmentCount = attachment_count;
        framebuffer_create_info.pAttachments = attachments.data();
        framebuffer_create_info.width = swap_chain_extent.width;
        framebuffer_create_info.height = swap_chain_extent.height;
        framebuffer_create_info.layers = 1;

        HYP_VULKAN_CHECK(vkCreateFramebuffer(device->GetDevice(), &framebuffer_create_info, nullptr, &m_framebuffer));
    }

    //--------------------------------------------------------------
    VulkanGraphicsFramebuffer::~VulkanGraphicsFramebuffer() {
        VulkanGraphicsDevice *device = reinterpret_cast<VulkanGraphicsDevice *>(GetDevice());
        vkDestroyFramebuffer(device->GetDevice(), m_framebuffer, nullptr);
    }

}