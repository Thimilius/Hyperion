#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsUtilities final {
    public:
        static void ReportError(VkResult result);
    private:
        VulkanGraphicsUtilities() = delete;
        ~VulkanGraphicsUtilities() = delete;
    };

}

#define HYP_VULKAN_CHECK(x)                                                   \
    do {                                                                      \
        VkResult result = (x);                                                \
        if (result != VK_SUCCESS) {                                           \
            HYP_DEBUG_BREAK;                                                  \
            Hyperion::Graphics::VulkanGraphicsUtilities::ReportError(result); \
        }                                                                     \
    } while (false)