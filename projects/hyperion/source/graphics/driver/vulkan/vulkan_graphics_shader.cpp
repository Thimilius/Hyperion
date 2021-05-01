//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_shader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsShader::VulkanGraphicsShader(VulkanGraphicsDevice *device, const GraphicsShaderDescription &description) : GraphicsShader(device, description) {
        VkShaderModuleCreateInfo shader_module_create_info = { };
        shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = description.byte_code_size;
        shader_module_create_info.pCode = reinterpret_cast<const uint32_t *>(description.byte_code);

        HYP_VULKAN_CHECK(vkCreateShaderModule(device->GetDevice(), &shader_module_create_info, nullptr, &m_shader_module));
    }

    //--------------------------------------------------------------
    VulkanGraphicsShader::~VulkanGraphicsShader() {
        VulkanGraphicsDevice *device = static_cast<VulkanGraphicsDevice *>(GetDevice());
        vkDestroyShaderModule(device->GetDevice(), m_shader_module, nullptr);
    }

}