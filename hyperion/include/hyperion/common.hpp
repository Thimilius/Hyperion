#pragma once

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <set>

#define BIT(x) (unsigned)(1 << x)

#ifdef _MSC_VER
    #define HYP_DEBUG_BREAK __debugbreak()
#else
    #define HYP_DEBUG_BREAK
#endif

#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/string_utils.hpp"
#include "core/enum.hpp"
#include "system/log.hpp"
