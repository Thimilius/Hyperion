#include "hyppch.hpp"

#include "hyperion/math/vec3.hpp"

namespace Hyperion::Math {

    SVec3::SVec3()
        : x(0), y(0), z(0) {
    }

    SVec3::SVec3(float x, float y, float z)
        : x(x), y(y), z(z) {
    }

    SVec3 &SVec3::Add(const SVec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    SVec3 &SVec3::Subtract(const SVec3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    SVec3 &SVec3::Multiply(const SVec3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    SVec3 &SVec3::Divide(const SVec3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    SVec3 &SVec3::Add(float value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    SVec3 &SVec3::Subtract(float value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    SVec3 &SVec3::Multiply(float value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    SVec3 &SVec3::Divide(float value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    SVec3 SVec3::Multiply(const SMat4 &transform) const {
        return SVec3(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z
        );
    }

    TString SVec3::ToString() const {
        return CStringUtils::Format("({:.2f}, {:.2f}, {:.2f})", x, y, z);
    }

    SVec3 operator+(SVec3 left, const SVec3 &right) {
        return left.Add(right);
    }

    SVec3 operator-(SVec3 left, const SVec3 &right) {
        return left.Subtract(right);
    }

    SVec3 operator*(SVec3 left, const SVec3 &right) {
        return left.Multiply(right);
    }

    SVec3 operator/(SVec3 left, const SVec3 &right) {
        return left.Divide(right);
    }

    SVec3 operator+(SVec3 left, float value) {
        return left.Add(value);
    }

    SVec3 operator-(SVec3 left, float value) {
        return left.Subtract(value);
    }

    SVec3 operator*(SVec3 left, float value) {
        return left.Multiply(value);
    }

    SVec3 operator/(SVec3 left, float value) {
        return left.Divide(value);
    }

    bool SVec3::operator==(const SVec3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool SVec3::operator!=(const SVec3 &other) const {
        return !(*this == other);
    }

    SVec3 &SVec3::operator+=(const SVec3 &other) {
        return Add(other);
    }

    SVec3 &SVec3::operator-=(const SVec3 &other) {
        return Subtract(other);
    }

    SVec3 &SVec3::operator*=(const SVec3 &other) {
        return Multiply(other);
    }

    SVec3 &SVec3::operator/=(const SVec3 &other) {
        return Divide(other);
    }

    bool SVec3::operator<(const SVec3 &other) const {
        return x < other.x && y < other.y && z < other.z;
    }

    bool SVec3::operator<=(const SVec3 &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool SVec3::operator>(const SVec3 &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    bool SVec3::operator>=(const SVec3 &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    float SVec3::Dot(const SVec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    SVec3 SVec3::Normalized() const {
        float magnitude = Magnitude();
        if (magnitude == 0) {
            return SVec3();
        } else {
            return SVec3(x / magnitude, y / magnitude, z / magnitude);
        }
    }

    float SVec3::Magnitude() const {
        return CMathf::Sqrt(x * x + y * y + z * z);
    }

    float SVec3::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    float SVec3::Distance(const SVec3 &other) const {
        float x = this->x - other.x;
        float y = this->y - other.y;
        float z = this->z - other.z;

        return CMathf::Sqrt(x * x + y * y + z * z);
    }

    SVec3 SVec3::Cross(const SVec3 &a, const SVec3 &b) {
        float x = a.y * b.z - a.z * b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x * b.y - a.y * b.x;

        return SVec3(x, y, z);
    }

}