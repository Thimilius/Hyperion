#pragma once

#include <stdint.h>

//#include "util/log.h"

#ifdef _MSC_VER
    #define HYP_DEBUG_BREAK __debugbreak()
#else
    #error
#endif

#ifdef HYP_DEBUG
    #define HYP_ASSERT(x) do { if(!x) { HYP_LOG_ERROR("***Assertion failed!*** %s (%d)", __FILE__, __LINE__); HYP_DEBUG_BREAK; } } while(0)
#else
    #define HYP_ASSERT(x)
#endif