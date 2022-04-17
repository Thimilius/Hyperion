//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanShaderCompiler {
  public:
    static VkShaderModule CompileModule(VkDevice device, const String &file_path, VkShaderStageFlagBits stage);
  private:
    VulkanShaderCompiler() = delete;
    ~VulkanShaderCompiler() = delete;
  };

}