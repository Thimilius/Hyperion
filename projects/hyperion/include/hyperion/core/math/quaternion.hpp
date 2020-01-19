#pragma once

#include "hyperion/core/math/vec3.hpp"

namespace Hyperion {

    struct Quaternion {
        f32 x;
        f32 y;
        f32 z;
        f32 w;

        Quaternion();
        Quaternion(f32 x, f32 y, f32 z, f32 w);
        Quaternion(Vec3 xyz, f32 w);

        Vec3 ToEulerAngles() const;

        Quaternion Normalized() const;
        Quaternion Inverted() const;

        String ToString() const;

        bool operator==(const Quaternion &other) const;
        bool operator!=(const Quaternion &other) const;

        Quaternion operator-() const;

        Quaternion operator+(const Quaternion &other) const;
        Quaternion operator-(const Quaternion &other) const;
        Quaternion operator*(const Quaternion &other) const;
        Vec3 operator*(const Vec3 &other) const;
        Quaternion operator*(f32 value) const;
        Quaternion operator/(f32 value) const;

        static Vec3 Rotate(const Quaternion &quaternion, const Vec3 &vec);

        static Quaternion FromEulerAngles(const Vec3 &angles);
        static Quaternion FromEulerAngles(f32 x, f32 y, f32 z);

        inline static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }
    };

}