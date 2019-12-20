#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Vec2 {
        float x;
        float y;

        Vec2();
        Vec2(float x, float y);

        Vec2 &Add(const Vec2 &other);
        Vec2 &Subtract(const Vec2 &other);
        Vec2 &Multiply(const Vec2 &other);
        Vec2 &Divide(const Vec2 &other);

        Vec2 &Add(float value);
        Vec2 &Subtract(float value);
        Vec2 &Multiply(float value);
        Vec2 &Divide(float value);

        String ToString() const;

        friend Vec2 operator+(Vec2 left, const Vec2 &right);
        friend Vec2 operator-(Vec2 left, const Vec2 &right);
        friend Vec2 operator*(Vec2 left, const Vec2 &right);
        friend Vec2 operator/(Vec2 left, const Vec2 &right);

        friend Vec2 operator+(Vec2 left, float value);
        friend Vec2 operator-(Vec2 left, float value);
        friend Vec2 operator*(Vec2 left, float value);
        friend Vec2 operator/(Vec2 left, float value);

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

        float Dot(const Vec2 &other) const;

        Vec2 Normalized() const;

        float Magnitude() const;
        float SqrMagnitude() const;

        float Distance(const Vec2 &other) const;
    };

}