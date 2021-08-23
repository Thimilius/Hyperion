#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <cstdint>
#include <vector>
#include <unordered_set>

//--------------------------------------------------------------
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using byte = uint8;

using float32 = float;
using float64 = double;

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using Set = std::unordered_set<T>;