#pragma once

#include <cmath>

#include "hyperion/core/types.hpp"

namespace Hyperion {

    class Math final {
    public:
        inline static constexpr float32 PI = 3.1415926535897932384626433833f;
        inline static constexpr float32 TAU = PI * 2.0f;
        inline static constexpr float32 EPSILON = 0.001f;
        inline static constexpr float32 E = 2.7182818284590452353602874714f;

        inline static float32 Max(float32 a, float32 b) { return a > b ? a : b; }
        inline static uint32 Max(uint32 a, uint32 b) { return a > b ? a : b; }
        inline static float32 Min(float32 a, float32 b) { return a < b ? a : b; }
        inline static uint32 Min(uint32 a, uint32 b) { return a < b ? a : b; }
        inline static float32 Abs(float32 p_val) { return std::abs(p_val); }

        inline static float32 Sin(float32 p_rad) { return std::sin(p_rad); }
        inline static float32 Cos(float32 p_rad) { return std::cos(p_rad); }
        inline static float32 Tan(float32 p_rad) { return std::tan(p_rad); }

        inline static float32 ASin(float32 p_val) { return std::asin(p_val); }
        inline static float32 ACos(float32 p_val) { return std::acos(p_val); }
        inline static float32 ATan(float32 p_val) { return std::atan(p_val); }
        inline static float32 ATan2(float32 p_a, float32 p_b) { return std::atan2(p_a, p_b); }

        inline static float32 Round(float32 p_val) { return std::round(p_val); }
        inline static float32 Floor(float32 p_val) { return std::floor(p_val); }
        inline static float32 Ceil(float32 p_val) { return std::ceil(p_val); }

        inline static float32 Sqrt(float32 p_val) { return std::sqrt(p_val); }
        inline static float32 FMod(float32 p_a, float32 p_b) { return std::fmod(p_a, p_b); }
        inline static float32 ModF(float32 x, float32 m) { return x - static_cast<uint32>(x / m) * m; }

        inline static float32 Pow(float32 p_base, float32 p_expo) { return std::pow(p_base, p_expo); }
        inline static float32 Log(float32 p_val) { return std::log(p_val); }
        inline static float32 Log2(float32 p_val) { return std::log2(p_val); }
        inline static float32 Exp(float32 p_val) { return std::exp(p_val); }

        inline static float32 DegToRad(float32 p_deg) { return p_deg * PI / 180.0f; }
        inline static float32 RadToDeg(float32 p_rad) { return p_rad * 180.0f / PI; }

        inline static float32 Clamp(float32 p_val, float32 p_min, float32 p_max) {
            if (p_val < p_min) {
                return p_min;
            } else if (p_val > p_max) {
                return p_max;
            } else {
                return p_val;
            }
        }

        inline static float32 Clamp01(float32 p_val) {
            if (p_val < 0.0f) {
                return 0.0f;
            } else if (p_val > 1.0f) {
                return 1.0f;
            } else {
                return p_val;
            }
        }

        inline static float32 Lerp(float32 p_from, float32 p_to, float32 p_t) {
            p_t = Clamp01(p_t);
            return (1.0f - p_t) * p_from + p_t * p_to;
        }

        inline static float32 LerpUnclamped(float32 p_from, float32 p_to, float32 p_t) {
            return (1.0f - p_t) * p_from + p_t * p_to;
        }

        inline static float32 InverseLerp(float32 p_from, float32 p_to, float32 p_val) {
            return (p_val - p_from) / (p_to - p_from);
        }

        inline static float32 Smoothstep(float32 p_from, float32 p_to, float32 p_t) {
            p_t = Clamp01((p_t - p_from) / (p_to - p_from));
            return p_t * p_t * (3.0f - 2.0f * p_t);
        }
    private:
        Math() = delete;
        ~Math() = delete;
    };

}