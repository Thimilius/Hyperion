#include "hyppch.hpp"

#include "hyperion/math/vec2.hpp"

namespace Hyperion::Math {

    SVec2::SVec2()
        : x(0), y(0) {
    }

    SVec2::SVec2(float x, float y)
        : x(x), y(y) {
    }

    SVec2 &SVec2::Add(const SVec2 &other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    SVec2 &SVec2::Subtract(const SVec2 &other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    SVec2 &SVec2::Multiply(const SVec2 &other) {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    SVec2 &SVec2::Divide(const SVec2 &other) {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    SVec2 &SVec2::Add(float value) {
        x += value;
        y += value;

        return *this;
    }

    SVec2 &SVec2::Subtract(float value) {
        x -= value;
        y -= value;

        return *this;
    }

    SVec2 &SVec2::Multiply(float value) {
        x *= value;
        y *= value;

        return *this;
    }

    SVec2 &SVec2::Divide(float value) {
        x /= value;
        y /= value;

        return *this;
    }

    CString SVec2::ToString() const {
        return CStringUtils::Format("({:.2f}, {:.2f})", x, y);
    }

    SVec2 operator+(SVec2 left, const SVec2 &right) {
        return left.Add(right);
    }

    SVec2 operator-(SVec2 left, const SVec2 &right) {
        return left.Subtract(right);
    }

    SVec2 operator*(SVec2 left, const SVec2 &right) {
        return left.Multiply(right);
    }

    SVec2 operator/(SVec2 left, const SVec2 &right) {
        return left.Divide(right);
    }

    SVec2 operator+(SVec2 left, float value) {
        return left.Add(value);
    }

    SVec2 operator-(SVec2 left, float value) {
        return left.Subtract(value);
    }

    SVec2 operator*(SVec2 left, float value) {
        return left.Multiply(value);
    }

    SVec2 operator/(SVec2 left, float value) {
        return left.Divide(value);
    }

    bool SVec2::operator==(const SVec2 &other) const {
        return x == other.x && y == other.y;
    }

    bool SVec2::operator!=(const SVec2 &other) const {
        return !(*this == other);
    }

    SVec2 &SVec2::operator+=(const SVec2 &other) {
        return Add(other);
    }

    SVec2 &SVec2::operator-=(const SVec2 &other) {
        return Subtract(other);
    }

    SVec2 &SVec2::operator*=(const SVec2 &other) {
        return Multiply(other);
    }

    SVec2 &SVec2::operator/=(const SVec2 &other) {
        return Divide(other);
    }

    bool SVec2::operator<(const SVec2 &other) const {
        return x < other.x && y < other.y;
    }

    bool SVec2::operator<=(const SVec2 &other) const {
        return x <= other.x && y <= other.y;
    }

    bool SVec2::operator>(const SVec2 &other) const {
        return x > other.x && y > other.y;
    }

    bool SVec2::operator>=(const SVec2 &other) const {
        return x >= other.x && y >= other.y;
    }

    float SVec2::Dot(const SVec2 &other) const {
        return x * other.x + y * other.y;
    }

    SVec2 SVec2::Normalized() const {
        float magnitude = Magnitude();
        if (magnitude == 0) {
            return SVec2();
        } else {
            return SVec2(x / magnitude, y / magnitude);
        }
    }

    float SVec2::Magnitude() const {
        return CMathf::Sqrt(x * x + y * y);
    }

    float SVec2::SqrMagnitude() const {
        return x * x + y * y;
    }

    float SVec2::Distance(const SVec2 &other) const {
        float x = this->x - other.x;
        float y = this->y - other.y;

        return CMathf::Sqrt(x * x + y * y);
    }

}