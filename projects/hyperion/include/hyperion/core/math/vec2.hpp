#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec3;

    struct Vec2 {
        float32 x;
        float32 y;

        Vec2();
        Vec2(float32 x, float32 y);
        Vec2(const Vec3 &vec3);

        Vec2 &Add(const Vec2 &other);
        Vec2 &Subtract(const Vec2 &other);
        Vec2 &Multiply(const Vec2 &other);
        Vec2 &Divide(const Vec2 &other);

        Vec2 &Add(float32 value);
        Vec2 &Subtract(float32 value);
        Vec2 &Multiply(float32 value);
        Vec2 &Divide(float32 value);

        String ToString() const;

        bool operator==(const Vec2 &other) const;
        bool operator!=(const Vec2 &other) const;

        Vec2 &operator-();

        Vec2 &operator+=(const Vec2 &other);
        Vec2 &operator-=(const Vec2 &other);
        Vec2 &operator*=(const Vec2 &other);
        Vec2 &operator/=(const Vec2 &other);

        bool operator<(const Vec2 &other) const;
        bool operator<=(const Vec2 &other) const;
        bool operator>(const Vec2 &other) const;
        bool operator>=(const Vec2 &other) const;

        float32 Dot(const Vec2 &other) const;

        Vec2 Normalized() const;

        float32 Magnitude() const;
        float32 SqrMagnitude() const;

        float32 Distance(const Vec2 &other) const;

        friend Vec2 operator+(Vec2 left, const Vec2 &right);
        friend Vec2 operator-(Vec2 left, const Vec2 &right);
        friend Vec2 operator*(Vec2 left, const Vec2 &right);
        friend Vec2 operator/(Vec2 left, const Vec2 &right);

        friend Vec2 operator+(Vec2 left, float32 right);
        friend Vec2 operator-(Vec2 left, float32 right);
        friend Vec2 operator*(Vec2 left, float32 right);
        friend Vec2 operator/(Vec2 left, float32 right);

        friend Vec2 operator+(float32 left, Vec2 right);
        friend Vec2 operator-(float32 left, Vec2 right);
        friend Vec2 operator*(float32 left, Vec2 right);
        friend Vec2 operator/(float32 left, Vec2 right);
    };

}