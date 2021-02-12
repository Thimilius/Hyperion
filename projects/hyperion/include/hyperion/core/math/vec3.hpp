#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    struct Vec2;
    struct Vec2Int;
    struct Vec4;
    struct Mat4;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {
  
    struct Vec3 {
        float32 x;
        float32 y;
        float32 z;

        Vec3();
        Vec3(float32 x, float32 y, float32 z);
        Vec3(const Vec2 &vec2, float32 z);
        Vec3(const Vec4 &vec4);

        Vec3 &Add(const Vec3 &other);
        Vec3 &Subtract(const Vec3 &other);
        Vec3 &Multiply(const Vec3 &other);
        Vec3 &Divide(const Vec3 &other);

        Vec3 &Add(float32 value);
        Vec3 &Subtract(float32 value);
        Vec3 &Multiply(float32 value);
        Vec3 &Divide(float32 value);

        Vec3 Multiply(const Mat4 &transform) const;

        float32 Dot(const Vec3 &other) const;

        Vec3 Normalized() const;

        float32 Magnitude() const;
        float32 SqrMagnitude() const;

        float32 Distance(const Vec3 &other) const;

        String ToString() const;

        bool operator==(const Vec3 &other) const;
        bool operator!=(const Vec3 &other) const;

        Vec3 &operator-();

        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator*=(const Vec3 &other);
        Vec3 &operator/=(const Vec3 &other);

        bool operator<(const Vec3 &other) const;
        bool operator<=(const Vec3 &other) const;
        bool operator>(const Vec3 &other) const;
        bool operator>=(const Vec3 &other) const;

        static Vec3 Cross(const Vec3 &a, const Vec3 &b);

        inline static Vec3 Zero() { return Vec3(0, 0, 0); }
        inline static Vec3 One() { return Vec3(1, 1, 1); }
        inline static Vec3 Up() { return Vec3(0, 1, 0); }
        inline static Vec3 Down() { return Vec3(0, -1, 0); }
        inline static Vec3 Right() { return Vec3(1, 0, 0); }
        inline static Vec3 Left() { return Vec3(-1, 0, 0); }
        inline static Vec3 Forward() { return Vec3(0, 0, -1); }
        inline static Vec3 Back() { return Vec3(0, 0, 1); }

        friend Vec3 operator+(Vec3 left, const Vec3 &right);
        friend Vec3 operator-(Vec3 left, const Vec3 &right);
        friend Vec3 operator*(Vec3 left, const Vec3 &right);
        friend Vec3 operator/(Vec3 left, const Vec3 &right);

        friend Vec3 operator+(Vec3 left, float32 right);
        friend Vec3 operator-(Vec3 left, float32 right);
        friend Vec3 operator*(Vec3 left, float32 right);
        friend Vec3 operator/(Vec3 left, float32 right);

        friend Vec3 operator+(float32 left, Vec3 right);
        friend Vec3 operator-(float32 left, Vec3 right);
        friend Vec3 operator*(float32 left, Vec3 right);
        friend Vec3 operator/(float32 left, Vec3 right);
    };

    struct Vec3Int {
        int32 x;
        int32 y;
        int32 z;

        Vec3Int();
        Vec3Int(int32 x, int32 y, int32 z);
        Vec3Int(const Vec2Int &vec2, int32 z);

        Vec3Int &Add(const Vec3Int &other);
        Vec3Int &Subtract(const Vec3Int &other);
        Vec3Int &Multiply(const Vec3Int &other);
        Vec3Int &Divide(const Vec3Int &other);

        Vec3Int &Add(int32 value);
        Vec3Int &Subtract(int32 value);
        Vec3Int &Multiply(int32 value);
        Vec3Int &Divide(int32 value);

        float32 Magnitude() const;
        int32 SqrMagnitude() const;

        float32 Distance(const Vec3Int &other) const;

        String ToString() const;

        bool operator==(const Vec3Int &other) const;
        bool operator!=(const Vec3Int &other) const;

        Vec3Int &operator-();

        Vec3Int &operator+=(const Vec3Int &other);
        Vec3Int &operator-=(const Vec3Int &other);
        Vec3Int &operator*=(const Vec3Int &other);
        Vec3Int &operator/=(const Vec3Int &other);

        bool operator<(const Vec3Int &other) const;
        bool operator<=(const Vec3Int &other) const;
        bool operator>(const Vec3Int &other) const;
        bool operator>=(const Vec3Int &other) const;

        inline static Vec3Int Zero() { return Vec3Int(0, 0, 0); }
        inline static Vec3Int One() { return Vec3Int(1, 1, 1); }
        inline static Vec3Int Up() { return Vec3Int(0, 1, 0); }
        inline static Vec3Int Down() { return Vec3Int(0, -1, 0); }
        inline static Vec3Int Right() { return Vec3Int(1, 0, 0); }
        inline static Vec3Int Left() { return Vec3Int(-1, 0, 0); }
        inline static Vec3Int Forward() { return Vec3Int(0, 0, -1); }
        inline static Vec3Int Back() { return Vec3Int(0, 0, 1); }

        friend Vec3Int operator+(Vec3Int left, const Vec3Int &right);
        friend Vec3Int operator-(Vec3Int left, const Vec3Int &right);
        friend Vec3Int operator*(Vec3Int left, const Vec3Int &right);
        friend Vec3Int operator/(Vec3Int left, const Vec3Int &right);

        friend Vec3Int operator+(Vec3Int left, int32 right);
        friend Vec3Int operator-(Vec3Int left, int32 right);
        friend Vec3Int operator*(Vec3Int left, int32 right);
        friend Vec3Int operator/(Vec3Int left, int32 right);

        friend Vec3Int operator+(int32 left, Vec3Int right);
        friend Vec3Int operator-(int32 left, Vec3Int right);
        friend Vec3Int operator*(int32 left, Vec3Int right);
        friend Vec3Int operator/(int32 left, Vec3Int right);
    };

}