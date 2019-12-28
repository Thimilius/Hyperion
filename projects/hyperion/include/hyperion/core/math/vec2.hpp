#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec3;

    struct Vec2 {
        f32 x;
        f32 y;

        Vec2();
        Vec2(f32 x, f32 y);
        Vec2(const Vec3 &vec3);

        Vec2 &Add(const Vec2 &other);
        Vec2 &Subtract(const Vec2 &other);
        Vec2 &Multiply(const Vec2 &other);
        Vec2 &Divide(const Vec2 &other);

        Vec2 &Add(f32 value);
        Vec2 &Subtract(f32 value);
        Vec2 &Multiply(f32 value);
        Vec2 &Divide(f32 value);

        String ToString() const;

        bool operator==(const Vec2 &other) const;
        bool operator!=(const Vec2 &other) const;

        Vec2 &operator+=(const Vec2 &other);
        Vec2 &operator-=(const Vec2 &other);
        Vec2 &operator*=(const Vec2 &other);
        Vec2 &operator/=(const Vec2 &other);

        bool operator<(const Vec2 &other) const;
        bool operator<=(const Vec2 &other) const;
        bool operator>(const Vec2 &other) const;
        bool operator>=(const Vec2 &other) const;

        f32 Dot(const Vec2 &other) const;

        Vec2 Normalized() const;

        f32 Magnitude() const;
        f32 SqrMagnitude() const;

        f32 Distance(const Vec2 &other) const;

        friend Vec2 operator+(Vec2 left, const Vec2 &right);
        friend Vec2 operator-(Vec2 left, const Vec2 &right);
        friend Vec2 operator*(Vec2 left, const Vec2 &right);
        friend Vec2 operator/(Vec2 left, const Vec2 &right);

        friend Vec2 operator+(Vec2 left, f32 right);
        friend Vec2 operator-(Vec2 left, f32 right);
        friend Vec2 operator*(Vec2 left, f32 right);
        friend Vec2 operator/(Vec2 left, f32 right);

        friend Vec2 operator+(f32 left, Vec2 right);
        friend Vec2 operator-(f32 left, Vec2 right);
        friend Vec2 operator*(f32 left, Vec2 right);
        friend Vec2 operator/(f32 left, Vec2 right);
    };

}