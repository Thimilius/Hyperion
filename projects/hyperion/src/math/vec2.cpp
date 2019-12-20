#include "hyppch.hpp"

#include "hyperion/math/vec2.hpp"

namespace Hyperion {

    Vec2::Vec2()
        : x(0), y(0) {
    }

    Vec2::Vec2(float x, float y)
        : x(x), y(y) {
    }

    Vec2 &Vec2::Add(const Vec2 &other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    Vec2 &Vec2::Subtract(const Vec2 &other) {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Vec2 &Vec2::Multiply(const Vec2 &other) {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    Vec2 &Vec2::Divide(const Vec2 &other) {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    Vec2 &Vec2::Add(float value) {
        x += value;
        y += value;

        return *this;
    }

    Vec2 &Vec2::Subtract(float value) {
        x -= value;
        y -= value;

        return *this;
    }

    Vec2 &Vec2::Multiply(float value) {
        x *= value;
        y *= value;

        return *this;
    }

    Vec2 &Vec2::Divide(float value) {
        x /= value;
        y /= value;

        return *this;
    }

    String Vec2::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f})", x, y);
    }

    Vec2 operator+(Vec2 left, const Vec2 &right) {
        return left.Add(right);
    }

    Vec2 operator-(Vec2 left, const Vec2 &right) {
        return left.Subtract(right);
    }

    Vec2 operator*(Vec2 left, const Vec2 &right) {
        return left.Multiply(right);
    }

    Vec2 operator/(Vec2 left, const Vec2 &right) {
        return left.Divide(right);
    }

    Vec2 operator+(Vec2 left, float value) {
        return left.Add(value);
    }

    Vec2 operator-(Vec2 left, float value) {
        return left.Subtract(value);
    }

    Vec2 operator*(Vec2 left, float value) {
        return left.Multiply(value);
    }

    Vec2 operator/(Vec2 left, float value) {
        return left.Divide(value);
    }

    bool Vec2::operator==(const Vec2 &other) const {
        return x == other.x && y == other.y;
    }

    bool Vec2::operator!=(const Vec2 &other) const {
        return !(*this == other);
    }

    Vec2 &Vec2::operator+=(const Vec2 &other) {
        return Add(other);
    }

    Vec2 &Vec2::operator-=(const Vec2 &other) {
        return Subtract(other);
    }

    Vec2 &Vec2::operator*=(const Vec2 &other) {
        return Multiply(other);
    }

    Vec2 &Vec2::operator/=(const Vec2 &other) {
        return Divide(other);
    }

    bool Vec2::operator<(const Vec2 &other) const {
        return x < other.x && y < other.y;
    }

    bool Vec2::operator<=(const Vec2 &other) const {
        return x <= other.x && y <= other.y;
    }

    bool Vec2::operator>(const Vec2 &other) const {
        return x > other.x && y > other.y;
    }

    bool Vec2::operator>=(const Vec2 &other) const {
        return x >= other.x && y >= other.y;
    }

    float Vec2::Dot(const Vec2 &other) const {
        return x * other.x + y * other.y;
    }

    Vec2 Vec2::Normalized() const {
        float magnitude = Magnitude();
        if (magnitude == 0) {
            return Vec2();
        } else {
            return Vec2(x / magnitude, y / magnitude);
        }
    }

    float Vec2::Magnitude() const {
        return Mathf::Sqrt(x * x + y * y);
    }

    float Vec2::SqrMagnitude() const {
        return x * x + y * y;
    }

    float Vec2::Distance(const Vec2 &other) const {
        float x = this->x - other.x;
        float y = this->y - other.y;

        return Mathf::Sqrt(x * x + y * y);
    }

}