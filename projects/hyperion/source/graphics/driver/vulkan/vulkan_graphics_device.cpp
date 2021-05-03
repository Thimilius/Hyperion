//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_pipeline_state.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_framebuffer.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_render_pass.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsDevice::VulkanGraphicsDevice(VulkanGraphicsContext *context, VkDevice device, VkQueue graphics_queue) {
        m_context = context;
        m_device = device;
        m_graphics_queue = graphics_queue;
    }

    //--------------------------------------------------------------
    VulkanGraphicsDevice::~VulkanGraphicsDevice() {
        vkDestroyDevice(m_device, nullptr);
    }

    //--------------------------------------------------------------
    GraphicsBuffer *VulkanGraphicsDevice::CreateBuffer(const GraphicsBufferDescription &description) {
        return nullptr;
    }

    //--------------------------------------------------------------
    GraphicsTexture *VulkanGraphicsDevice::CreateTexture(const GraphicsTextureDescription &description) {
        return nullptr;
    }

    //--------------------------------------------------------------
    GraphicsShader *VulkanGraphicsDevice::CreateShader(const GraphicsShaderDescription &description) {
        return new VulkanGraphicsShader(this, description);
    }

    //--------------------------------------------------------------
    GraphicsRenderPass *VulkanGraphicsDevice::CreateRenderPass(const GraphicsRenderPassDescription &description) {
        return new VulkanGraphicsRenderPass(this, description);
    }

    //--------------------------------------------------------------
    GraphicsPipelineState *VulkanGraphicsDevice::CreatePipelineState(const GraphicsPipelineStateDescription &description) {
        return new VulkanGraphicsPipelineState(this, description);
    }

    GraphicsFramebuffer *VulkanGraphicsDevice::CreateFramebuffer(const GraphicsFramebufferDescription &description) {
        return new VulkanGraphicsFramebuffer(this, description);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsDevice::DestroyObject(GraphicsDeviceObject *object) {
        delete object;
    }

}