#pragma once

#include <cstdint>

namespace Hyperion {

    using s8 = int8_t;
    using s16 = int16_t;
    using s32 = int32_t;
    using s64 = int64_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

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