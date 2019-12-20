#pragma once

namespace Hyperion {

    class Mathf {
    public:
        static const f32 PI;
        static const f32 TAU;
        static const f32 EPSILON;

        static f32 Max(f32 a, f32 b);
        static f32 Min(f32 a, f32 b);

        static f32 Clamp(f32 value, f32 min, f32 max);
        static f32 Clamp01(f32 value);

        static f32 Lerp(f32 a, f32 b, f32 t);

        static f32 Sin(f32 value);
        static f32 Cos(f32 value);
        static f32 Tan(f32 value);

        static f32 ToRadians(f32 degrees);
        static f32 ToDegree(f32 radians);

        static f32 Sqrt(f32 value);
    private:
        Mathf() = delete;
        ~Mathf() = delete;
    };

}