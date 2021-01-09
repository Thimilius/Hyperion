#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

#include <rttr/type.h>

using s8 = int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

template<typename T>
using Vector = std::vector<T>;

template<typename Key, typename Value>
using Map = std::unordered_map<Key, Value>;

template<typename T>
using Set = std::unordered_set<T>;

template<typename T>
using Queue = std::queue<T>;

using Type = rttr::type;