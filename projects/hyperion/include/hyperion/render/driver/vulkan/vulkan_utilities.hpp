//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

#define HYP_VULKAN_CHECK(x, m) do {                                                                                            \
        if(x != VK_SUCCESS) {                                                                                                  \
            HYP_LOG_ERROR("Vulkan", m);                                                                                        \
            HYP_ASSERT_DEBUG_BREAK;                                                                                            \
        } } while(false)

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {



}