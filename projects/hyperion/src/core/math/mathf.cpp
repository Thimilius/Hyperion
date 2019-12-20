#include "hyppch.hpp"

#include "hyperion/core/math/mathf.hpp"

namespace Hyperion {

    const f32 Mathf::PI = 3.14159265358979323846f;
    const f32 Mathf::TAU = Mathf::PI * 2.0f;
    const f32 Mathf::EPSILON = 2.71828182845904523536f;

    f32 Mathf::Max(f32 a, f32 b) {
        return a > b ? a : b;
    }

    f32 Mathf::Min(f32 a, f32 b) {
        return a < b ? a : b;
    }

    f32 Mathf::Clamp(f32 value, f32 min, f32 max) {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        } else {
            return value;
        }
    }

    f32 Mathf::Clamp01(f32 value) {
        if (value < 0) {
            return 0.0f;
        } else if (value > 1) {
            return 1.0f;
        } else {
            return value;
        }
    }

    f32 Mathf::Lerp(f32 a, f32 b, f32 t) {
        t = Clamp01(t);
        return (1 - t) * a + t * b;
    }

    f32 Mathf::Sin(f32 value) {
        return sin(value);
    }

    f32 Mathf::Cos(f32 value) {
        return cos(value);
    }

    f32 Mathf::Tan(f32 value) {
        return tan(value);
    }

    f32 Mathf::ToRadians(f32 degrees) {
        return degrees * PI / 180.0f;
    }

    f32 Mathf::ToDegree(f32 radians) {
        return radians * 180.0f / PI;
    }

    f32 Mathf::Sqrt(f32 value) {
        return sqrt(value);
    }

}