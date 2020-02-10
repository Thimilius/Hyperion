#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec3;
    struct Mat4;
    struct Color;

    struct Vec4 {
        f32 x;
        f32 y;
        f32 z;
        f32 w;

        Vec4();
        Vec4(f32 x, f32 y, f32 z, f32 w);
        Vec4(const Vec3 &vec3, f32 w);

        Vec4 &Add(const Vec4 &other);
        Vec4 &Subtract(const Vec4 &other);
        Vec4 &Multiply(const Vec4 &other);
        Vec4 &Divide(const Vec4 &other);

        Vec4 &Add(f32 value);
        Vec4 &Subtract(f32 value);
        Vec4 &Multiply(f32 value);
        Vec4 &Divide(f32 value);

        Vec4 Multiply(const Mat4 &transform) const;

        f32 Dot(const Vec4 &other) const;

        String ToString() const;

        bool operator==(const Vec4 &other) const;
        bool operator!=(const Vec4 &other) const;

        Vec4 &operator-();

        Vec4 &operator+=(const Vec4 &other);
        Vec4 &operator-=(const Vec4 &other);
        Vec4 &operator*=(const Vec4 &other);
        Vec4 &operator/=(const Vec4 &other);

        bool operator<(const Vec4 &other) const;
        bool operator<=(const Vec4 &other) const;
        bool operator>(const Vec4 &other) const;
        bool operator>=(const Vec4 &other) const;

        operator Color();

        friend Vec4 operator+(Vec4 left, const Vec4 &right);
        friend Vec4 operator-(Vec4 left, const Vec4 &right);
        friend Vec4 operator*(Vec4 left, const Vec4 &right);
        friend Vec4 operator/(Vec4 left, const Vec4 &right);

        friend Vec4 operator+(Vec4 left, f32 right);
        friend Vec4 operator-(Vec4 left, f32 right);
        friend Vec4 operator*(Vec4 left, f32 right);
        friend Vec4 operator/(Vec4 left, f32 right);

        friend Vec4 operator+(f32 left, Vec4 right);
        friend Vec4 operator-(f32 left, Vec4 right);
        friend Vec4 operator*(f32 left, Vec4 right);
        friend Vec4 operator/(f32 left, Vec4 right);
    };

}