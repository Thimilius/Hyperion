#pragma once

#include <stdint.h>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#include "core/string.hpp"
#include "system/log.hpp"

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

    #define HYP_ASSERT(x, m) do {\
        if(!(x)) {\
            HYP_CORE_ERROR("[Assert] - Assertion failed!\nIn file: %s (%d)\nWith assertion message: %s", __FILE__, __LINE__, m);\
            HYP_ASSERT_DEBUG_BREAK;\
            abort();\
        } } while(0)
    #define HYP_ASSERT_ENUM_OUT_OF_RAGE HYP_ASSERT(false, "Enum out of range!")
#endif