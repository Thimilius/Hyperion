#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

#include "core/types.hpp"
#include "core/string_utils.hpp"
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
#else
    #define HYP_ASSERT_MESSAGE(x, m)
    #define HYP_ASSERT_ENUM_OUT_OF_RANGE
#endif

namespace Hyperion {

    template<typename T>
    using TRef = std::shared_ptr<T>;

    template<typename T>
    using TScope = std::unique_ptr<T>;

    template<typename T>
    using TVector = std::vector<T>;

    template<typename Key, typename Value>
    using TMap = std::unordered_map<Key, Value>;

}