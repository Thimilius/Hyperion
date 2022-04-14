//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

#ifdef HYP_DEBUG
#define HYP_VULKAN_CHECK(x, m) do {                                                                                            \
        if(x != VK_SUCCESS) {                                                                                                  \
            HYP_LOG_ERROR("Vulkan", m);                                                                                        \
            HYP_ASSERT_DEBUG_BREAK;                                                                                            \
        } } while(false)
#else
#define HYP_VULKAN_CHECK(x, m) x
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {}
