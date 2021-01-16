#include "hyppch.hpp"

#include "hyperion/core/math/vec2.hpp"

namespace Hyperion {

    Vec2::Vec2()
        : x(0), y(0) { }

    Vec2::Vec2(float32 x, float32 y)
        : x(x), y(y) { }

    Vec2::Vec2(const Vec3 &vec3) 
        : x(vec3.x), y(vec3.y) { }

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

    Vec2 &Vec2::Add(float32 value) {
        x += value;
        y += value;

        return *this;
    }

    Vec2 &Vec2::Subtract(float32 value) {
        x -= value;
        y -= value;

        return *this;
    }

    Vec2 &Vec2::Multiply(float32 value) {
        x *= value;
        y *= value;

        return *this;
    }

    Vec2 &Vec2::Divide(float32 value) {
        x /= value;
        y /= value;

        return *this;
    }

    String Vec2::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f})", x, y);
    }

    bool Vec2::operator==(const Vec2 &other) const {
        return x == other.x && y == other.y;
    }

    bool Vec2::operator!=(const Vec2 &other) const {
        return !(*this == other);
    }

    Vec2 &Vec2::operator-() {
        x = -x;
        y = -y;

        return *this;
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

    float32 Vec2::Dot(const Vec2 &other) const {
        return x * other.x + y * other.y;
    }

    Vec2 Vec2::Normalized() const {
        float32 magnitude = Magnitude();
        if (magnitude == 0) {
            return Vec2();
        } else {
            return Vec2(x / magnitude, y / magnitude);
        }
    }

    float32 Vec2::Magnitude() const {
        return Math::Sqrt(x * x + y * y);
    }

    float32 Vec2::SqrMagnitude() const {
        return x * x + y * y;
    }

    float32 Vec2::Distance(const Vec2 &other) const {
        float32 x = this->x - other.x;
        float32 y = this->y - other.y;

        return Math::Sqrt(x * x + y * y);
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

    Vec2 operator+(Vec2 left, float32 right) {
        return left.Add(right);
    }

    Vec2 operator-(Vec2 left, float32 right) {
        return left.Subtract(right);
    }

    Vec2 operator*(Vec2 left, float32 right) {
        return left.Multiply(right);
    }

    Vec2 operator/(Vec2 left, float32 right) {
        return left.Divide(right);
    }

    Vec2 operator+(float32 left, Vec2 right) {
        return right.Add(left);
    }

    Vec2 operator-(float32 left, Vec2 right) {
        return right.Subtract(left);
    }

    Vec2 operator*(float32 left, Vec2 right) {
        return right.Multiply(left);
    }

    Vec2 operator/(float32 left, Vec2 right) {
        return right.Divide(left);
    }

}