#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Math {

    struct SVec2 {
        float x;
        float y;

        SVec2();
        SVec2(float x, float y);

        SVec2 &Add(const SVec2 &other);
        SVec2 &Subtract(const SVec2 &other);
        SVec2 &Multiply(const SVec2 &other);
        SVec2 &Divide(const SVec2 &other);

        SVec2 &Add(float value);
        SVec2 &Subtract(float value);
        SVec2 &Multiply(float value);
        SVec2 &Divide(float value);

        CString ToString() const;

        friend SVec2 operator+(SVec2 left, const SVec2 &right);
        friend SVec2 operator-(SVec2 left, const SVec2 &right);
        friend SVec2 operator*(SVec2 left, const SVec2 &right);
        friend SVec2 operator/(SVec2 left, const SVec2 &right);

        friend SVec2 operator+(SVec2 left, float value);
        friend SVec2 operator-(SVec2 left, float value);
        friend SVec2 operator*(SVec2 left, float value);
        friend SVec2 operator/(SVec2 left, float value);

        bool operator==(const SVec2 &other) const;
        bool operator!=(const SVec2 &other) const;

        SVec2 &operator+=(const SVec2 &other);
        SVec2 &operator-=(const SVec2 &other);
        SVec2 &operator*=(const SVec2 &other);
        SVec2 &operator/=(const SVec2 &other);

        bool operator<(const SVec2 &other) const;
        bool operator<=(const SVec2 &other) const;
        bool operator>(const SVec2 &other) const;
        bool operator>=(const SVec2 &other) const;

        float Dot(const SVec2 &other) const;

        SVec2 Normalized() const;

        float Magnitude() const;
        float SqrMagnitude() const;

        float Distance(const SVec2 &other) const;
    };

}