#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsDevice : public GraphicsDevice {
    public:
        VulkanGraphicsDevice(VkDevice device, VkQueue graphics_queue);
        ~VulkanGraphicsDevice();
    public:
        inline VkDevice GetDevice() const { return m_device; }

        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) override;
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) override;
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description) override;
        GraphicsRenderPass *CreateRenderPass(const GraphicsRenderPassDescription &description) override;
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) override;

        void DestroyObject(GraphicsDeviceObject *object) override;
    private:
        VkDevice m_device;
        VkQueue m_graphics_queue;
    };

}