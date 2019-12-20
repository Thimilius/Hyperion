#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>

#define BIT(x) (unsigned)(1 << x)

#ifdef HYP_DEBUG
    #ifdef _MSC_VER
        #define HYP_DEBUG_BREAK __debugbreak()
    #else
        #define HYP_DEBUG_BREAK
    #endif
#else
    #define HYP_DEBUG_BREAK
#endif

#include "hyperion/core/types.hpp"
#include "hyperion/core/assert.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/enum.hpp"
#include "hyperion/core/log.hpp"

#define HYP_PANIC do {                                                \
            HYP_LOG_ERROR("Engine", "Engine encountered an error!");  \
            HYP_DEBUG_BREAK;                                          \
            Engine::Panic("Engine", "Engine encountered an error!"); \
        } while(false);
#define HYP_PANIC_MESSAGE(s, m, ...) do {          \
            HYP_LOG_ERROR((s), (m), __VA_ARGS__);  \
            HYP_DEBUG_BREAK;                       \
            Engine::Panic((s), (m), __VA_ARGS__); \
        } while(false);
