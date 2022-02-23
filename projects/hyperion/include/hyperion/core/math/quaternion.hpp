//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct Quaternion {
    float32 x;
    float32 y;
    float32 z;
    float32 w;

    Quaternion();
    Quaternion(float32 x, float32 y, float32 z, float32 w);
    Quaternion(Vector3 xyz, float32 w);

    Vector3 ToEulerAngles() const;

    Quaternion Normalized() const;
    Quaternion Inverted() const;

    String ToString() const;

    bool8 operator==(const Quaternion &other) const;
    bool8 operator!=(const Quaternion &other) const;

    Quaternion operator-() const;

    Quaternion operator+(const Quaternion &other) const;
    Quaternion operator-(const Quaternion &other) const;
    Quaternion operator*(const Quaternion &other) const;
    Vector3 operator*(const Vector3 &other) const;
    Quaternion operator*(float32 value) const;
    Quaternion operator/(float32 value) const;

    static Vector3 Rotate(const Quaternion &quaternion, const Vector3 &vec);

    static Quaternion FromAxisAngle(const Vector3 &axis, float32 angle);
    static Quaternion FromEulerAngles(const Vector3 &angles);
    static Quaternion FromEulerAngles(float32 x, float32 y, float32 z);

    inline static Quaternion Identity() { return Quaternion(0, 0, 0, 1); }
  };

}