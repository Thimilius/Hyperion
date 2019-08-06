#pragma once

namespace Hyperion::Math {

    class CMathf {
    public:
        static const float PI;
        static const float TAU;
        static const float EPSILON;

        static float Max(float a, float b);
        static float Min(float a, float b);

        static float Clamp(float value, float min, float max);
        static float Clamp01(float value);

        static float Lerp(float a, float b, float t);

        static float Sin(float value);
        static float Cos(float value);
        static float Tan(float value);

        static float ToRadians(float degrees);
        static float ToDegree(float radians);

        static float Sqrt(float value);
    private:
        CMathf() = delete;
        ~CMathf() = delete;
    };

}