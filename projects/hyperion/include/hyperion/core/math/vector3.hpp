//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    struct Vector2;
    struct Vector2Int;
    struct Vector4;
    struct Matrix4x4;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {
  
    struct Vector3 {
        float32 x;
        float32 y;
        float32 z;

        Vector3();
        Vector3(float32 x, float32 y, float32 z);
        Vector3(const Vector2 &Vector2, float32 z);
        Vector3(const Vector4 &vec4);

        Vector3 &Add(const Vector3 &other);
        Vector3 &Subtract(const Vector3 &other);
        Vector3 &Multiply(const Vector3 &other);
        Vector3 &Divide(const Vector3 &other);

        Vector3 &Add(float32 value);
        Vector3 &Subtract(float32 value);
        Vector3 &Multiply(float32 value);
        Vector3 &Divide(float32 value);

        Vector3 Multiply(const Matrix4x4 &transform) const;

        float32 Dot(const Vector3 &other) const;

        Vector3 Normalized() const;

        float32 Magnitude() const;
        float32 SqrMagnitude() const;

        float32 Distance(const Vector3 &other) const;

        String ToString() const;

        bool8 operator==(const Vector3 &other) const;
        bool8 operator!=(const Vector3 &other) const;

        Vector3 operator-();

        Vector3 &operator+=(const Vector3 &other);
        Vector3 &operator-=(const Vector3 &other);
        Vector3 &operator*=(const Vector3 &other);
        Vector3 &operator/=(const Vector3 &other);

        bool8 operator<(const Vector3 &other) const;
        bool8 operator<=(const Vector3 &other) const;
        bool8 operator>(const Vector3 &other) const;
        bool8 operator>=(const Vector3 &other) const;

        static Vector3 Cross(const Vector3 &a, const Vector3 &b);

        inline static Vector3 Zero() { return Vector3(0, 0, 0); }
        inline static Vector3 One() { return Vector3(1, 1, 1); }
        inline static Vector3 Up() { return Vector3(0, 1, 0); }
        inline static Vector3 Down() { return Vector3(0, -1, 0); }
        inline static Vector3 Right() { return Vector3(1, 0, 0); }
        inline static Vector3 Left() { return Vector3(-1, 0, 0); }
        inline static Vector3 Forward() { return Vector3(0, 0, -1); }
        inline static Vector3 Back() { return Vector3(0, 0, 1); }

        friend Vector3 operator+(Vector3 left, const Vector3 &right);
        friend Vector3 operator-(Vector3 left, const Vector3 &right);
        friend Vector3 operator*(Vector3 left, const Vector3 &right);
        friend Vector3 operator/(Vector3 left, const Vector3 &right);

        friend Vector3 operator+(Vector3 left, float32 right);
        friend Vector3 operator-(Vector3 left, float32 right);
        friend Vector3 operator*(Vector3 left, float32 right);
        friend Vector3 operator/(Vector3 left, float32 right);

        friend Vector3 operator+(float32 left, Vector3 right);
        friend Vector3 operator-(float32 left, Vector3 right);
        friend Vector3 operator*(float32 left, Vector3 right);
        friend Vector3 operator/(float32 left, Vector3 right);
    };

    struct Vector3Int {
        int32 x;
        int32 y;
        int32 z;

        Vector3Int();
        Vector3Int(int32 x, int32 y, int32 z);
        Vector3Int(const Vector2Int &Vector2, int32 z);

        Vector3Int &Add(const Vector3Int &other);
        Vector3Int &Subtract(const Vector3Int &other);
        Vector3Int &Multiply(const Vector3Int &other);
        Vector3Int &Divide(const Vector3Int &other);

        Vector3Int &Add(int32 value);
        Vector3Int &Subtract(int32 value);
        Vector3Int &Multiply(int32 value);
        Vector3Int &Divide(int32 value);

        float32 Magnitude() const;
        int32 SqrMagnitude() const;

        float32 Distance(const Vector3Int &other) const;

        String ToString() const;

        bool8 operator==(const Vector3Int &other) const;
        bool8 operator!=(const Vector3Int &other) const;

        Vector3Int operator-();

        Vector3Int &operator+=(const Vector3Int &other);
        Vector3Int &operator-=(const Vector3Int &other);
        Vector3Int &operator*=(const Vector3Int &other);
        Vector3Int &operator/=(const Vector3Int &other);

        bool8 operator<(const Vector3Int &other) const;
        bool8 operator<=(const Vector3Int &other) const;
        bool8 operator>(const Vector3Int &other) const;
        bool8 operator>=(const Vector3Int &other) const;

        inline static Vector3Int Zero() { return Vector3Int(0, 0, 0); }
        inline static Vector3Int One() { return Vector3Int(1, 1, 1); }
        inline static Vector3Int Up() { return Vector3Int(0, 1, 0); }
        inline static Vector3Int Down() { return Vector3Int(0, -1, 0); }
        inline static Vector3Int Right() { return Vector3Int(1, 0, 0); }
        inline static Vector3Int Left() { return Vector3Int(-1, 0, 0); }
        inline static Vector3Int Forward() { return Vector3Int(0, 0, -1); }
        inline static Vector3Int Back() { return Vector3Int(0, 0, 1); }

        friend Vector3Int operator+(Vector3Int left, const Vector3Int &right);
        friend Vector3Int operator-(Vector3Int left, const Vector3Int &right);
        friend Vector3Int operator*(Vector3Int left, const Vector3Int &right);
        friend Vector3Int operator/(Vector3Int left, const Vector3Int &right);

        friend Vector3Int operator+(Vector3Int left, int32 right);
        friend Vector3Int operator-(Vector3Int left, int32 right);
        friend Vector3Int operator*(Vector3Int left, int32 right);
        friend Vector3Int operator/(Vector3Int left, int32 right);

        friend Vector3Int operator+(int32 left, Vector3Int right);
        friend Vector3Int operator-(int32 left, Vector3Int right);
        friend Vector3Int operator*(int32 left, Vector3Int right);
        friend Vector3Int operator/(int32 left, Vector3Int right);
    };

}