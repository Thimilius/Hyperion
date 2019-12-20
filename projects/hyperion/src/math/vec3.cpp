#include "hyppch.hpp"

#include "hyperion/math/vec3.hpp"

namespace Hyperion {

    Vec3::Vec3()
        : x(0), y(0), z(0) {
    }

    Vec3::Vec3(float x, float y, float z)
        : x(x), y(y), z(z) {
    }

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

    Vec3 &Vec3::Add(float value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    Vec3 &Vec3::Subtract(float value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    Vec3 &Vec3::Multiply(float value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3 &Vec3::Divide(float value) {
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

    Vec3 operator+(Vec3 left, float value) {
        return left.Add(value);
    }

    Vec3 operator-(Vec3 left, float value) {
        return left.Subtract(value);
    }

    Vec3 operator*(Vec3 left, float value) {
        return left.Multiply(value);
    }

    Vec3 operator/(Vec3 left, float value) {
        return left.Divide(value);
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

    float Vec3::Dot(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 Vec3::Normalized() const {
        float magnitude = Magnitude();
        if (magnitude == 0) {
            return Vec3();
        } else {
            return Vec3(x / magnitude, y / magnitude, z / magnitude);
        }
    }

    float Vec3::Magnitude() const {
        return Mathf::Sqrt(x * x + y * y + z * z);
    }

    float Vec3::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    float Vec3::Distance(const Vec3 &other) const {
        float x = this->x - other.x;
        float y = this->y - other.y;
        float z = this->z - other.z;

        return Mathf::Sqrt(x * x + y * y + z * z);
    }

    Vec3 Vec3::Cross(const Vec3 &a, const Vec3 &b) {
        float x = a.y * b.z - a.z * b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x * b.y - a.y * b.x;

        return Vec3(x, y, z);
    }

}