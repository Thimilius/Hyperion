#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    struct Mat4;

    struct Vec3 {
        float x;
        float y;
        float z;

        Vec3();
        Vec3(float x, float y, float z);

        Vec3 &Add(const Vec3 &other);
        Vec3 &Subtract(const Vec3 &other);
        Vec3 &Multiply(const Vec3 &other);
        Vec3 &Divide(const Vec3 &other);

        Vec3 &Add(float value);
        Vec3 &Subtract(float value);
        Vec3 &Multiply(float value);
        Vec3 &Divide(float value);

        Vec3 Multiply(const Mat4 &transform) const;

        String ToString() const;

        friend Vec3 operator+(Vec3 left, const Vec3 &right);
        friend Vec3 operator-(Vec3 left, const Vec3 &right);
        friend Vec3 operator*(Vec3 left, const Vec3 &right);
        friend Vec3 operator/(Vec3 left, const Vec3 &right);

        friend Vec3 operator+(Vec3 left, float value);
        friend Vec3 operator-(Vec3 left, float value);
        friend Vec3 operator*(Vec3 left, float value);
        friend Vec3 operator/(Vec3 left, float value);

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

        float Dot(const Vec3 &other) const;

        Vec3 Normalized() const;

        float Magnitude() const;
        float SqrMagnitude() const;

        float Distance(const Vec3 &other) const;

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