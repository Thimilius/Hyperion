#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    struct Vec3;
    struct Mat4;
    struct Color;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct Vec4 {
        float32 x;
        float32 y;
        float32 z;
        float32 w;

        Vec4();
        Vec4(float32 x, float32 y, float32 z, float32 w);
        Vec4(const Vec3 &vec3, float32 w);

        Vec4 &Add(const Vec4 &other);
        Vec4 &Subtract(const Vec4 &other);
        Vec4 &Multiply(const Vec4 &other);
        Vec4 &Divide(const Vec4 &other);

        Vec4 &Add(float32 value);
        Vec4 &Subtract(float32 value);
        Vec4 &Multiply(float32 value);
        Vec4 &Divide(float32 value);

        Vec4 Multiply(const Mat4 &transform) const;

        float32 Dot(const Vec4 &other) const;

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

        friend Vec4 operator+(Vec4 left, float32 right);
        friend Vec4 operator-(Vec4 left, float32 right);
        friend Vec4 operator*(Vec4 left, float32 right);
        friend Vec4 operator/(Vec4 left, float32 right);

        friend Vec4 operator+(float32 left, Vec4 right);
        friend Vec4 operator-(float32 left, Vec4 right);
        friend Vec4 operator*(float32 left, Vec4 right);
        friend Vec4 operator/(float32 left, Vec4 right);
    };

}