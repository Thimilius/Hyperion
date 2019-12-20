#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Mat4;

    struct Vec4 {
        float x;
        float y;
        float z;
        float w;

        Vec4();
        Vec4(float x, float y, float z, float w);

        Vec4 &Add(const Vec4 &other);
        Vec4 &Subtract(const Vec4 &other);
        Vec4 &Multiply(const Vec4 &other);
        Vec4 &Divide(const Vec4 &other);

        Vec4 &Add(float value);
        Vec4 &Subtract(float value);
        Vec4 &Multiply(float value);
        Vec4 &Divide(float value);

        Vec4 Multiply(const Mat4 &transform) const;

        String ToString() const;

        friend Vec4 operator+(Vec4 left, const Vec4 &right);
        friend Vec4 operator-(Vec4 left, const Vec4 &right);
        friend Vec4 operator*(Vec4 left, const Vec4 &right);
        friend Vec4 operator/(Vec4 left, const Vec4 &right);

        friend Vec4 operator+(Vec4 left, float value);
        friend Vec4 operator-(Vec4 left, float value);
        friend Vec4 operator*(Vec4 left, float value);
        friend Vec4 operator/(Vec4 left, float value);

        bool operator==(const Vec4 &other) const;
        bool operator!=(const Vec4 &other) const;

        Vec4 &operator+=(const Vec4 &other);
        Vec4 &operator-=(const Vec4 &other);
        Vec4 &operator*=(const Vec4 &other);
        Vec4 &operator/=(const Vec4 &other);

        bool operator<(const Vec4 &other) const;
        bool operator<=(const Vec4 &other) const;
        bool operator>(const Vec4 &other) const;
        bool operator>=(const Vec4 &other) const;

        float Dot(const Vec4 &other) const;
    };

}