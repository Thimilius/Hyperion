#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Math {

    struct SMat4;

    struct SVec3 {
        float x;
        float y;
        float z;

        SVec3();
        SVec3(float x, float y, float z);

        SVec3 &Add(const SVec3 &other);
        SVec3 &Subtract(const SVec3 &other);
        SVec3 &Multiply(const SVec3 &other);
        SVec3 &Divide(const SVec3 &other);

        SVec3 &Add(float value);
        SVec3 &Subtract(float value);
        SVec3 &Multiply(float value);
        SVec3 &Divide(float value);

        SVec3 Multiply(const SMat4 &transform) const;

        TString ToString() const;

        friend SVec3 operator+(SVec3 left, const SVec3 &right);
        friend SVec3 operator-(SVec3 left, const SVec3 &right);
        friend SVec3 operator*(SVec3 left, const SVec3 &right);
        friend SVec3 operator/(SVec3 left, const SVec3 &right);

        friend SVec3 operator+(SVec3 left, float value);
        friend SVec3 operator-(SVec3 left, float value);
        friend SVec3 operator*(SVec3 left, float value);
        friend SVec3 operator/(SVec3 left, float value);

        bool operator==(const SVec3 &other) const;
        bool operator!=(const SVec3 &other) const;

        SVec3 &operator+=(const SVec3 &other);
        SVec3 &operator-=(const SVec3 &other);
        SVec3 &operator*=(const SVec3 &other);
        SVec3 &operator/=(const SVec3 &other);

        bool operator<(const SVec3 &other) const;
        bool operator<=(const SVec3 &other) const;
        bool operator>(const SVec3 &other) const;
        bool operator>=(const SVec3 &other) const;

        SVec3 Cross(const SVec3 &other) const;
        float Dot(const SVec3 &other) const;

        SVec3 Normalize() const;

        float Magnitude() const;
        float SqrMagnitude() const;

        float Distance(const SVec3 &other) const;

        inline static SVec3 Zero() { return SVec3(0, 0, 0); }
        inline static SVec3 One() { return SVec3(1, 1, 1); }
        inline static SVec3 Up() { return SVec3(0, 1, 0); }
        inline static SVec3 Down() { return SVec3(0, -1, 0); }
        inline static SVec3 Right() { return SVec3(1, 0, 0); }
        inline static SVec3 Left() { return SVec3(-1, 0, 0); }
        inline static SVec3 Forward() { return SVec3(0, 0, 1); }
        inline static SVec3 Back() { return SVec3(0, 0, -1); }
    };

}