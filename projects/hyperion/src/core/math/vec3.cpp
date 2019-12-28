#include "hyppch.hpp"

namespace Hyperion {

    Vec3::Vec3()
        : x(0), y(0), z(0) { }

    Vec3::Vec3(f32 x, f32 y, f32 z)
        : x(x), y(y), z(z) { }

    Vec3::Vec3(const Vec2 &vec2, f32 z)
        : x(vec2.x), y(vec2.y), z(z) { }

    Vec3::Vec3(const Vec4 &vec4)
        : x(vec4.x), y(vec4.y), z(vec4.z) { }

    Vec3 &Vec3::Add(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3 &Vec3::Subtract(const Vec3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Vec3 &Vec3::Multiply(const Vec3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    Vec3 &Vec3::Divide(const Vec3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    Vec3 &Vec3::Add(f32 value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    Vec3 &Vec3::Subtract(f32 value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    Vec3 &Vec3::Multiply(f32 value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3 &Vec3::Divide(f32 value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    Vec3 Vec3::Multiply(const Mat4 &transform) const {
        return Vec3(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z
        );
    }

    String Vec3::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f})", x, y, z);
    }

    bool Vec3::operator==(const Vec3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Vec3::operator!=(const Vec3 &other) const {
        return !(*this == other);
    }

    Vec3 &Vec3::operator+=(const Vec3 &other) {
        return Add(other);
    }

    Vec3 &Vec3::operator-=(const Vec3 &other) {
        return Subtract(other);
    }

    Vec3 &Vec3::operator*=(const Vec3 &other) {
        return Multiply(other);
    }

    Vec3 &Vec3::operator/=(const Vec3 &other) {
        return Divide(other);
    }

    bool Vec3::operator<(const Vec3 &other) const {
        return x < other.x && y < other.y && z < other.z;
    }

    bool Vec3::operator<=(const Vec3 &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool Vec3::operator>(const Vec3 &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    bool Vec3::operator>=(const Vec3 &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    f32 Vec3::Dot(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 Vec3::Normalized() const {
        f32 magnitude = Magnitude();
        if (magnitude == 0) {
            return Vec3();
        } else {
            return Vec3(x / magnitude, y / magnitude, z / magnitude);
        }
    }

    f32 Vec3::Magnitude() const {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    f32 Vec3::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    f32 Vec3::Distance(const Vec3 &other) const {
        f32 x = this->x - other.x;
        f32 y = this->y - other.y;
        f32 z = this->z - other.z;

        return Math::Sqrt(x * x + y * y + z * z);
    }

    Vec3 Vec3::Cross(const Vec3 &a, const Vec3 &b) {
        f32 x = a.y * b.z - a.z * b.y;
        f32 y = a.z * b.x - a.x * b.z;
        f32 z = a.x * b.y - a.y * b.x;

        return Vec3(x, y, z);
    }

    Vec3 operator+(Vec3 left, const Vec3 &right) {
        return left.Add(right);
    }

    Vec3 operator-(Vec3 left, const Vec3 &right) {
        return left.Subtract(right);
    }

    Vec3 operator*(Vec3 left, const Vec3 &right) {
        return left.Multiply(right);
    }

    Vec3 operator/(Vec3 left, const Vec3 &right) {
        return left.Divide(right);
    }

    Vec3 operator+(Vec3 left, f32 right) {
        return left.Add(right);
    }

    Vec3 operator-(Vec3 left, f32 right) {
        return left.Subtract(right);
    }

    Vec3 operator*(Vec3 left, f32 right) {
        return left.Multiply(right);
    }

    Vec3 operator/(Vec3 left, f32 right) {
        return left.Divide(right);
    }

    Vec3 operator+(f32 left, Vec3 right) {
        return right.Add(left);
    }

    Vec3 operator-(f32 left, Vec3 right) {
        return right.Subtract(left);
    }

    Vec3 operator*(f32 left, Vec3 right) {
        return right.Multiply(left);
    }

    Vec3 operator/(f32 left, Vec3 right) {
        return right.Divide(left);
    }

}