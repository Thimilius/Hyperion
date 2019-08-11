#pragma once

#include <stdint.h>
#include <memory>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#include "core/string.hpp"
#include "core/enum.hpp"
#include "system/log.hpp"

#define BIT(x) (1 << x)

#ifdef _MSC_VER
    #define HYP_DEBUG_BREAK __debugbreak()
#else
    #define HYP_DEBUG_BREAK
#endif

#ifdef HYP_DEBUG
    #ifdef HYP_BREAK_ON_ASSERT
        #define HYP_ASSERT_DEBUG_BREAK HYP_DEBUG_BREAK
    #else
        #define HYP_ASSERT_DEBUG_BREAK
    #endif

    #define HYP_ASSERT_MESSAGE(x, m) do {\
        if(!(x)) {\
            HYP_CORE_ERROR("[Assert] - Assertion failed!\nIn file: {} ({})\nWith assertion message: {}", __FILE__, __LINE__, m);\
            HYP_ASSERT_DEBUG_BREAK;\
            abort();\
        } } while(0)
    #define HYP_ASSERT_ENUM_OUT_OF_RANGE HYP_ASSERT_MESSAGE(false, "Enum out of range!")
#endif

namespace Hyperion {

    template<typename T>
    using PRef = std::shared_ptr<T>;

    template<typename T>
    using PScope = std::unique_ptr<T>;

}