//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  struct Vector3;
  struct Vector3Int;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct Vector2 {
    float32 x;
    float32 y;

    Vector2();
    Vector2(float32 x, float32 y);
    Vector2(const Vector3 &Vector3);

    Vector2 &Add(const Vector2 &other);
    Vector2 &Subtract(const Vector2 &other);
    Vector2 &Multiply(const Vector2 &other);
    Vector2 &Divide(const Vector2 &other);

    Vector2 &Add(float32 value);
    Vector2 &Subtract(float32 value);
    Vector2 &Multiply(float32 value);
    Vector2 &Divide(float32 value);

    String ToString() const;

    bool8 operator==(const Vector2 &other) const;
    bool8 operator!=(const Vector2 &other) const;

    Vector2 operator-();

    Vector2 &operator+=(const Vector2 &other);
    Vector2 &operator-=(const Vector2 &other);
    Vector2 &operator*=(const Vector2 &other);
    Vector2 &operator/=(const Vector2 &other);

    bool8 operator<(const Vector2 &other) const;
    bool8 operator<=(const Vector2 &other) const;
    bool8 operator>(const Vector2 &other) const;
    bool8 operator>=(const Vector2 &other) const;

    float32 Dot(const Vector2 &other) const;

    Vector2 Normalized() const;

    float32 Magnitude() const;
    float32 SqrMagnitude() const;

    float32 Distance(const Vector2 &other) const;

    friend Vector2 operator+(Vector2 left, const Vector2 &right);
    friend Vector2 operator-(Vector2 left, const Vector2 &right);
    friend Vector2 operator*(Vector2 left, const Vector2 &right);
    friend Vector2 operator/(Vector2 left, const Vector2 &right);

    friend Vector2 operator+(Vector2 left, float32 right);
    friend Vector2 operator-(Vector2 left, float32 right);
    friend Vector2 operator*(Vector2 left, float32 right);
    friend Vector2 operator/(Vector2 left, float32 right);

    friend Vector2 operator+(float32 left, Vector2 right);
    friend Vector2 operator-(float32 left, Vector2 right);
    friend Vector2 operator*(float32 left, Vector2 right);
    friend Vector2 operator/(float32 left, Vector2 right);
  };

  struct Vector2Int {
    int32 x;
    int32 y;

    Vector2Int();
    Vector2Int(int32 x, int32 y);
    Vector2Int(const Vector3Int &Vector3);

    Vector2Int &Add(const Vector2Int &other);
    Vector2Int &Subtract(const Vector2Int &other);
    Vector2Int &Multiply(const Vector2Int &other);
    Vector2Int &Divide(const Vector2Int &other);

    Vector2Int &Add(int32 value);
    Vector2Int &Subtract(int32 value);
    Vector2Int &Multiply(int32 value);
    Vector2Int &Divide(int32 value);

    Vector2 ToFloat() const;
    String ToString() const;

    bool8 operator==(const Vector2Int &other) const;
    bool8 operator!=(const Vector2Int &other) const;

    Vector2Int operator-();

    Vector2Int &operator+=(const Vector2Int &other);
    Vector2Int &operator-=(const Vector2Int &other);
    Vector2Int &operator*=(const Vector2Int &other);
    Vector2Int &operator/=(const Vector2Int &other);

    bool8 operator<(const Vector2Int &other) const;
    bool8 operator<=(const Vector2Int &other) const;
    bool8 operator>(const Vector2Int &other) const;
    bool8 operator>=(const Vector2Int &other) const;

    int32 Dot(const Vector2Int &other) const;

    float32 Magnitude() const;
    int32 SqrMagnitude() const;

    float32 Distance(const Vector2Int &other) const;

    friend Vector2Int operator+(Vector2Int left, const Vector2Int &right);
    friend Vector2Int operator-(Vector2Int left, const Vector2Int &right);
    friend Vector2Int operator*(Vector2Int left, const Vector2Int &right);
    friend Vector2Int operator/(Vector2Int left, const Vector2Int &right);

    friend Vector2Int operator+(Vector2Int left, int32 right);
    friend Vector2Int operator-(Vector2Int left, int32 right);
    friend Vector2Int operator*(Vector2Int left, int32 right);
    friend Vector2Int operator/(Vector2Int left, int32 right);

    friend Vector2Int operator+(int32 left, Vector2Int right);
    friend Vector2Int operator-(int32 left, Vector2Int right);
    friend Vector2Int operator*(int32 left, Vector2Int right);
    friend Vector2Int operator/(int32 left, Vector2Int right);
  };

}