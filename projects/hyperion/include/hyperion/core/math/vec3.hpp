#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec2;
    struct Vec4;
    struct Mat4;

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

}