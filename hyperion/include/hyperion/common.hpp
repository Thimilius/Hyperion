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
    #error
#endif

#ifdef HYP_DEBUG
    #define HYP_ASSERT(x) do { if(!(x)) { HYP_CORE_ERROR("***Assertion failed!*** %s (%d)", __FILE__, __LINE__); HYP_DEBUG_BREAK; } } while(0)
#else
    #define HYP_ASSERT(x)
#endif