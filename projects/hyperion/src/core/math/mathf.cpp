#include "hyppch.hpp"

#include "hyperion/core/math/mathf.hpp"

namespace Hyperion {

    const float Mathf::PI = 3.14159265358979323846f;
    const float Mathf::TAU = Mathf::PI * 2.0f;
    const float Mathf::EPSILON = 2.71828182845904523536f;

    float Mathf::Max(float a, float b) {
        return a > b ? a : b;
    }

    float Mathf::Min(float a, float b) {
        return a < b ? a : b;
    }

    float Mathf::Clamp(float value, float min, float max) {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        } else {
            return value;
        }
    }

    float Mathf::Clamp01(float value) {
        if (value < 0) {
            return 0.0f;
        } else if (value > 1) {
            return 1.0f;
        } else {
            return value;
        }
    }

    float Mathf::Lerp(float a, float b, float t) {
        t = Clamp01(t);
        return (1 - t) * a + t * b;
    }

    float Mathf::Sin(float value) {
        return sin(value);
    }

    float Mathf::Cos(float value) {
        return cos(value);
    }

    float Mathf::Tan(float value) {
        return tan(value);
    }

    float Mathf::ToRadians(float degrees) {
        return degrees * PI / 180.0f;
    }

    float Mathf::ToDegree(float radians) {
        return radians * 180.0f / PI;
    }

    float Mathf::Sqrt(float value) {
        return sqrt(value);
    }

}