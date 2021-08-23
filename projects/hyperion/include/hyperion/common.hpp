#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <memory>
#include <functional>

#define BIT(x) (1 << x)

#ifdef HYP_DEBUG
    #ifdef _MSC_VER
        #define HYP_DEBUG_BREAK __debugbreak()
    #else
        #define HYP_DEBUG_BREAK
    #endif
#else
    #define HYP_DEBUG_BREAK
#endif

//---------------------- Project Includes ----------------------
#include "hyperion/core/assert.hpp"
#include "hyperion/core/collections/collections.hpp"
#include "hyperion/core/enum.hpp"
#include "hyperion/core/error.hpp"
#include "hyperion/core/non_copyable.hpp"
#include "hyperion/core/types.hpp"
#include "hyperion/core/result.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/system/log.hpp"
