#include "hyppch.hpp"

#include "hyperion/math/mathf.hpp"

namespace Hyperion::Math {

    const float CMathf::PI = 3.14159265358979323846f;
    const float CMathf::TAU = CMathf::PI * 2.0f;
    const float CMathf::EPSILON = 2.71828182845904523536f;

    float CMathf::Max(float a, float b) {
        return a > b ? a : b;
    }

    float CMathf::Min(float a, float b) {
        return a < b ? a : b;
    }

    float CMathf::Clamp(float value, float min, float max) {
        if (value < min) {
            return min;
        } else if (value > max) {
            return max;
        } else {
            return value;
        }
    }

    float CMathf::Clamp01(float value) {
        if (value < 0) {
            return 0.0f;
        } else if (value > 1) {
            return 1.0f;
        } else {
            return value;
        }
    }

    float CMathf::Lerp(float a, float b, float t) {
        t = Clamp01(t);
        return (1 - t) * a + t * b;
    }

    float CMathf::Sin(float value) {
        return sin(value);
    }

    float CMathf::Cos(float value) {
        return cos(value);
    }

    float CMathf::Tan(float value) {
        return tan(value);
    }

    float CMathf::ToRadians(float degrees) {
        return degrees * PI / 180.0f;
    }

    float CMathf::ToDegree(float radians) {
        return radians * 180.0f / PI;
    }

    float CMathf::Sqrt(float value) {
        return sqrt(value);
    }

}