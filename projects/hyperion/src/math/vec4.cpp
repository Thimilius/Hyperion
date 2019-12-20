#include "hyppch.hpp"

#include "hyperion/math/vec4.hpp"

namespace Hyperion {

    Vec4::Vec4()
        : x(0), y(0), z(0), w(0) {
    }

    Vec4::Vec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {
    }

    Vec4 &Vec4::Add(const Vec4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;

        return *this;
    }

    Vec4 &Vec4::Subtract(const Vec4 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;

        return *this;
    }

    Vec4 &Vec4::Multiply(const Vec4 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;

        return *this;
    }

    Vec4 &Vec4::Divide(const Vec4 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;

        return *this;
    }

    Vec4 &Vec4::Add(float value) {
        x += value;
        y += value;
        z += value;
        w += value;

        return *this;
    }

    Vec4 &Vec4::Subtract(float value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;

        return *this;
    }

    Vec4 &Vec4::Multiply(float value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;

        return *this;
    }

    Vec4 &Vec4::Divide(float value) {
        x /= value;
        y /= value;
        z /= value;
        w /= value;

        return *this;
    }

    Vec4 Vec4::Multiply(const Mat4 &transform) const {
        return Vec4(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x * w,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y * w,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z * w,
            transform.columns[0].w * x + transform.columns[1].w * y + transform.columns[2].w * z + transform.columns[3].w * w
        );
    }

    String Vec4::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})", x, y, z, w);
    }

    Vec4 operator+(Vec4 left, const Vec4 &right) {
        return left.Add(right);
    }

    Vec4 operator-(Vec4 left, const Vec4 &right) {
        return left.Subtract(right);
    }

    Vec4 operator*(Vec4 left, const Vec4 &right) {
        return left.Multiply(right);
    }

    Vec4 operator/(Vec4 left, const Vec4 &right) {
        return left.Divide(right);
    }

    Vec4 operator+(Vec4 left, float value) {
        return left.Add(value);
    }

    Vec4 operator-(Vec4 left, float value) {
        return left.Subtract(value);
    }

    Vec4 operator*(Vec4 left, float value) {
        return left.Multiply(value);
    }

    Vec4 operator/(Vec4 left, float value) {
        return left.Divide(value);
    }

    bool Vec4::operator==(const Vec4 &other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool Vec4::operator!=(const Vec4 &other) const {
        return !(*this == other);
    }

    Vec4 &Vec4::operator+=(const Vec4 &other) {
        return Add(other);
    }

    Vec4 &Vec4::operator-=(const Vec4 &other) {
        return Subtract(other);
    }

    Vec4 &Vec4::operator*=(const Vec4 &other) {
        return Multiply(other);
    }

    Vec4 &Vec4::operator/=(const Vec4 &other) {
        return Divide(other);
    }

    bool Vec4::operator<(const Vec4 &other) const {
        return x < other.x && y < other.y && z < other.z && w < other.w;
    }

    bool Vec4::operator<=(const Vec4 &other) const {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }

    bool Vec4::operator>(const Vec4 &other) const {
        return x > other.x && y > other.y && z > other.z && w > other.w;
    }

    bool Vec4::operator>=(const Vec4 &other) const {
        return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
    }

    float Vec4::Dot(const Vec4 &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

}