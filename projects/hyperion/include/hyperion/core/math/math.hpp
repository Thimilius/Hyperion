#pragma once

#include <cmath>

#include "hyperion/core/types.hpp"

namespace Hyperion {

    class Math {
    public:
        inline static const f32 PI = 3.1415926535897932384626433833f;
        inline static const f32 TAU = PI * 2.0f;
        inline static const f32 EPSILON = 0.001f;
        inline static const f32 E = 2.7182818284590452353602874714f;

        inline static f32 Max(f32 a, f32 b) { return a > b ? a : b; }
        inline static f32 Min(f32 a, f32 b) { return a < b ? a : b; }
        inline static f32 Abs(f32 p_val) { return std::abs(p_val); }

        inline static f32 Sin(f32 p_rad) { return std::sin(p_rad); }
        inline static f32 Cos(f32 p_rad) { return std::cos(p_rad); }
        inline static f32 Tan(f32 p_rad) { return std::tan(p_rad); }

        inline static f32 ASin(f32 p_val) { return std::asin(p_val); }
        inline static f32 ACos(f32 p_val) { return std::acos(p_val); }
        inline static f32 ATan(f32 p_val) { return std::atan(p_val); }
        inline static f32 ATan2(f32 p_a, f32 p_b) { return std::atan2(p_a, p_b); }

        inline static f32 Round(f32 p_val) { return std::round(p_val); }
        inline static f32 Floor(f32 p_val) { return std::floor(p_val); }
        inline static f32 Ceil(f32 p_val) { return std::ceil(p_val); }

        inline static f32 Sqrt(f32 p_val) { return std::sqrt(p_val); }
        inline static f32 Fmod(f32 p_a, f32 p_b) { return std::fmod(p_a, p_b); }

        inline static f32 Pow(f32 p_base, f32 p_expo) { return std::pow(p_base, p_expo); }
        inline static f32 Log(f32 p_val) { return std::log(p_val); }
        inline static f32 Log2(f32 p_val) { return std::log2(p_val); }
        inline static f32 Exp(f32 p_val) { return std::exp(p_val); }

        inline static f32 DegToRad(f32 p_deg) { return p_deg * PI / 180.0f; }
        inline static f32 RadToDeg(f32 p_rad) { return p_rad * 180.0f / PI; }

        inline static f32 Clamp(f32 p_val, f32 p_min, f32 p_max) {
            if (p_val < p_min) {
                return p_min;
            } else if (p_val > p_max) {
                return p_max;
            } else {
                return p_val;
            }
        }

        inline static f32 Clamp01(f32 p_val) {
            if (p_val < 0.0f) {
                return 0.0f;
            } else if (p_val > 1.0f) {
                return 1.0f;
            } else {
                return p_val;
            }
        }

        inline static f32 Lerp(f32 p_from, f32 p_to, f32 p_t) {
            p_t = Clamp01(p_t);
            return (1.0f - p_t) * p_from + p_t * p_to;
        }

        inline static f32 LerpUnclamped(f32 p_from, f32 p_to, f32 p_t) {
            return (1.0f - p_t) * p_from + p_t * p_to;
        }

        inline static f32 InverseLerp(f32 p_from, f32 p_to, f32 p_val) {
            return (p_val - p_from) / (p_to - p_from);
        }

        inline static f32 Smoothstep(f32 p_from, f32 p_to, f32 p_t) {
            p_t = Clamp01((p_t - p_from) / (p_to - p_from));
            return p_t * p_t * (3.0f - 2.0f * p_t);
        }
    private:
        Math() = delete;
        ~Math() = delete;
    };

}