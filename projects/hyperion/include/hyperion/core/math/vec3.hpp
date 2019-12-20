#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Mat4;

    struct Vec3 {
        f32 x;
        f32 y;
        f32 z;

        Vec3();
        Vec3(f32 x, f32 y, f32 z);

        Vec3 &Add(const Vec3 &other);
        Vec3 &Subtract(const Vec3 &other);
        Vec3 &Multiply(const Vec3 &other);
        Vec3 &Divide(const Vec3 &other);

        Vec3 &Add(f32 value);
        Vec3 &Subtract(f32 value);
        Vec3 &Multiply(f32 value);
        Vec3 &Divide(f32 value);

        Vec3 Multiply(const Mat4 &transform) const;

        String ToString() const;

        friend Vec3 operator+(Vec3 left, const Vec3 &right);
        friend Vec3 operator-(Vec3 left, const Vec3 &right);
        friend Vec3 operator*(Vec3 left, const Vec3 &right);
        friend Vec3 operator/(Vec3 left, const Vec3 &right);

        friend Vec3 operator+(Vec3 left, f32 value);
        friend Vec3 operator-(Vec3 left, f32 value);
        friend Vec3 operator*(Vec3 left, f32 value);
        friend Vec3 operator/(Vec3 left, f32 value);

        bool operator==(const Vec3 &other) const;
        bool operator!=(const Vec3 &other) const;

        Vec3 &operator+=(const Vec3 &other);
        Vec3 &operator-=(const Vec3 &other);
        Vec3 &operator*=(const Vec3 &other);
        Vec3 &operator/=(const Vec3 &other);

        bool operator<(const Vec3 &other) const;
        bool operator<=(const Vec3 &other) const;
        bool operator>(const Vec3 &other) const;
        bool operator>=(const Vec3 &other) const;

        f32 Dot(const Vec3 &other) const;

        Vec3 Normalized() const;

        f32 Magnitude() const;
        f32 SqrMagnitude() const;

        f32 Distance(const Vec3 &other) const;

        static Vec3 Cross(const Vec3 &a, const Vec3 &b);

        inline static Vec3 Zero() { return Vec3(0, 0, 0); }
        inline static Vec3 One() { return Vec3(1, 1, 1); }
        inline static Vec3 Up() { return Vec3(0, 1, 0); }
        inline static Vec3 Down() { return Vec3(0, -1, 0); }
        inline static Vec3 Right() { return Vec3(1, 0, 0); }
        inline static Vec3 Left() { return Vec3(-1, 0, 0); }
        inline static Vec3 Forward() { return Vec3(0, 0, -1); }
        inline static Vec3 Back() { return Vec3(0, 0, 1); }
    };

}