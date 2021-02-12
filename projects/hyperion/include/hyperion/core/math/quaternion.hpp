#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vec3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct Quaternion {
        float32 x;
        float32 y;
        float32 z;
        float32 w;

        Quaternion();
        Quaternion(float32 x, float32 y, float32 z, float32 w);
        Quaternion(Vec3 xyz, float32 w);

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
        Quaternion operator*(float32 value) const;
        Quaternion operator/(float32 value) const;

        static Vec3 Rotate(const Quaternion &quaternion, const Vec3 &vec);

        static Quaternion FromAxisAngle(const Vec3 &axis, float32 angle);
        static Quaternion FromEulerAngles(const Vec3 &angles);
        static Quaternion FromEulerAngles(float32 x, float32 y, float32 z);

        inline static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }
    };

}