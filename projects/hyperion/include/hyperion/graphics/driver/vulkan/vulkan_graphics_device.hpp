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
        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) { return nullptr; }
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) { return nullptr; }
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description) { return nullptr; }
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) { return nullptr; }

        void DestroyObject(GraphicsDeviceObject *object) { }
    private:
        VkDevice m_device;
        VkQueue m_graphics_queue;
    };

}