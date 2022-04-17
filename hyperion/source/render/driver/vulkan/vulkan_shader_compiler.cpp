//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_shader_compiler.hpp"

//---------------------- Library Includes ----------------------
#include "shaderc/shaderc.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  VkShaderModule VulkanShaderCompiler::CompileModule(VkDevice device, const String &file_path, VkShaderStageFlagBits stage) {
    const String &source = FileSystem::ReadAllText(file_path);

    shaderc_shader_kind shader_kind;
    switch (stage) {
      case VK_SHADER_STAGE_VERTEX_BIT: shader_kind = shaderc_vertex_shader; break;
      case VK_SHADER_STAGE_FRAGMENT_BIT: shader_kind = shaderc_fragment_shader; break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
    }

    shaderc::Compiler compiler;
    shaderc::SpvCompilationResult compilation_result = compiler.CompileGlslToSpv(source, shader_kind, file_path.c_str());
    if (compilation_result.GetCompilationStatus() != shaderc_compilation_status_success) {
      HYP_LOG_ERROR("Vulkan", "Failed to compile shader module!");
      return nullptr;
    }

    uint64 size = (compilation_result.end() - compilation_result.begin()) * sizeof(shaderc::SpvCompilationResult::element_type);

    VkShaderModuleCreateInfo create_info = { };
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = size;
    create_info.pCode = reinterpret_cast<const uint32 *>(compilation_result.begin());

    VkShaderModule shader_module;
    HYP_VULKAN_CHECK(vkCreateShaderModule(device, &create_info, nullptr, &shader_module), "Failed to create shader module!");
    return shader_module;
  }

}