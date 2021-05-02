#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_shader.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsDevice;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsShader : public GraphicsShader {
    public:
        VulkanGraphicsShader(VulkanGraphicsDevice *device, const GraphicsShaderDescription &description);
        ~VulkanGraphicsShader();
    public:
        VkShaderModule GetShaderModule() const { return m_shader_module; }
    private:
        VkShaderModule m_shader_module;
    };

}