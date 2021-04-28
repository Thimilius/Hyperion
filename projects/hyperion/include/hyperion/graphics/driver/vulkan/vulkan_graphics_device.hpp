#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsContext;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsDevice : public GraphicsDevice {
    public:
        VulkanGraphicsDevice(VulkanGraphicsContext *context, VkDevice device, VkQueue graphics_queue);
        ~VulkanGraphicsDevice();
    public:
        inline VulkanGraphicsContext *GetContext() const { return m_context; }

        inline VkDevice GetDevice() const { return m_device; }

        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) override;
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) override;
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description) override;
        GraphicsRenderPass *CreateRenderPass(const GraphicsRenderPassDescription &description) override;
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) override;

        void DestroyObject(GraphicsDeviceObject *object) override;
    private:
        VulkanGraphicsContext *m_context;

        VkDevice m_device;
        VkQueue m_graphics_queue;
    };

}