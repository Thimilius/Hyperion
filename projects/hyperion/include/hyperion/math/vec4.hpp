#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Math {

    struct SMat4;

    struct SVec4 {
        float x;
        float y;
        float z;
        float w;

        SVec4();
        SVec4(float x, float y, float z, float w);

        SVec4 &Add(const SVec4 &other);
        SVec4 &Subtract(const SVec4 &other);
        SVec4 &Multiply(const SVec4 &other);
        SVec4 &Divide(const SVec4 &other);

        SVec4 &Add(float value);
        SVec4 &Subtract(float value);
        SVec4 &Multiply(float value);
        SVec4 &Divide(float value);

        SVec4 Multiply(const SMat4 &transform) const;

        TString ToString() const;

        friend SVec4 operator+(SVec4 left, const SVec4 &right);
        friend SVec4 operator-(SVec4 left, const SVec4 &right);
        friend SVec4 operator*(SVec4 left, const SVec4 &right);
        friend SVec4 operator/(SVec4 left, const SVec4 &right);

        friend SVec4 operator+(SVec4 left, float value);
        friend SVec4 operator-(SVec4 left, float value);
        friend SVec4 operator*(SVec4 left, float value);
        friend SVec4 operator/(SVec4 left, float value);

        bool operator==(const SVec4 &other) const;
        bool operator!=(const SVec4 &other) const;

        SVec4 &operator+=(const SVec4 &other);
        SVec4 &operator-=(const SVec4 &other);
        SVec4 &operator*=(const SVec4 &other);
        SVec4 &operator/=(const SVec4 &other);

        bool operator<(const SVec4 &other) const;
        bool operator<=(const SVec4 &other) const;
        bool operator>(const SVec4 &other) const;
        bool operator>=(const SVec4 &other) const;

        float Dot(const SVec4 &other) const;
    };

}