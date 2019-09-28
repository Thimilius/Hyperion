#pragma once

#include <cstdint>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

namespace Hyperion {

    template<typename T>
    using TRef = std::shared_ptr<T>;

    template<typename T>
    using TScope = std::unique_ptr<T>;

    template<typename T>
    using TVector = std::vector<T>;

    template<typename Key, typename Value>
    using TMap = std::unordered_map<Key, Value>;

    template<typename T>
    using TSet = std::set<T>;

}