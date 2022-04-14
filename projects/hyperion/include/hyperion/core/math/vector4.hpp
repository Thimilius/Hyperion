//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  struct Vector3;
  struct Matrix4x4;
  struct Color;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct Vector4 {
    float32 x;
    float32 y;
    float32 z;
    float32 w;

    Vector4();
    Vector4(float32 x, float32 y, float32 z, float32 w);
    Vector4(const Vector3 &vec3, float32 w);

    Vector4 &Add(const Vector4 &other);
    Vector4 &Subtract(const Vector4 &other);
    Vector4 &Multiply(const Vector4 &other);
    Vector4 &Divide(const Vector4 &other);

    Vector4 &Add(float32 value);
    Vector4 &Subtract(float32 value);
    Vector4 &Multiply(float32 value);
    Vector4 &Divide(float32 value);

    Vector4 Multiply(const Matrix4x4 &transform) const;

    float32 Dot(const Vector4 &other) const;

    String ToString() const;

    bool8 operator==(const Vector4 &other) const;
    bool8 operator!=(const Vector4 &other) const;

    Vector4 operator-();

    Vector4 &operator+=(const Vector4 &other);
    Vector4 &operator-=(const Vector4 &other);
    Vector4 &operator*=(const Vector4 &other);
    Vector4 &operator/=(const Vector4 &other);

    bool8 operator<(const Vector4 &other) const;
    bool8 operator<=(const Vector4 &other) const;
    bool8 operator>(const Vector4 &other) const;
    bool8 operator>=(const Vector4 &other) const;

    operator Color();

    friend Vector4 operator+(Vector4 left, const Vector4 &right);
    friend Vector4 operator-(Vector4 left, const Vector4 &right);
    friend Vector4 operator*(Vector4 left, const Vector4 &right);
    friend Vector4 operator/(Vector4 left, const Vector4 &right);

    friend Vector4 operator+(Vector4 left, float32 right);
    friend Vector4 operator-(Vector4 left, float32 right);
    friend Vector4 operator*(Vector4 left, float32 right);
    friend Vector4 operator/(Vector4 left, float32 right);

    friend Vector4 operator+(float32 left, Vector4 right);
    friend Vector4 operator-(float32 left, Vector4 right);
    friend Vector4 operator*(float32 left, Vector4 right);
    friend Vector4 operator/(float32 left, Vector4 right);
  };

}
