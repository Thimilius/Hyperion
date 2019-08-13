#include "hyppch.hpp"

#include "hyperion/math/vec4.hpp"

namespace Hyperion::Math {

    SVec4::SVec4()
        : x(0), y(0), z(0), w(0) {
    }

    SVec4::SVec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {
    }

    SVec4 &SVec4::Add(const SVec4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;

        return *this;
    }

    SVec4 &SVec4::Subtract(const SVec4 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;

        return *this;
    }

    SVec4 &SVec4::Multiply(const SVec4 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;

        return *this;
    }

    SVec4 &SVec4::Divide(const SVec4 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;

        return *this;
    }

    SVec4 &SVec4::Add(float value) {
        x += value;
        y += value;
        z += value;
        w += value;

        return *this;
    }

    SVec4 &SVec4::Subtract(float value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;

        return *this;
    }

    SVec4 &SVec4::Multiply(float value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;

        return *this;
    }

    SVec4 &SVec4::Divide(float value) {
        x /= value;
        y /= value;
        z /= value;
        w /= value;

        return *this;
    }

    SVec4 SVec4::Multiply(const SMat4 &transform) const {
        return SVec4(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x * w,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y * w,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z * w,
            transform.columns[0].w * x + transform.columns[1].w * y + transform.columns[2].w * z + transform.columns[3].w * w
        );
    }

    TString SVec4::ToString() const {
        return CStringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})", x, y, z, w);
    }

    SVec4 operator+(SVec4 left, const SVec4 &right) {
        return left.Add(right);
    }

    SVec4 operator-(SVec4 left, const SVec4 &right) {
        return left.Subtract(right);
    }

    SVec4 operator*(SVec4 left, const SVec4 &right) {
        return left.Multiply(right);
    }

    SVec4 operator/(SVec4 left, const SVec4 &right) {
        return left.Divide(right);
    }

    SVec4 operator+(SVec4 left, float value) {
        return left.Add(value);
    }

    SVec4 operator-(SVec4 left, float value) {
        return left.Subtract(value);
    }

    SVec4 operator*(SVec4 left, float value) {
        return left.Multiply(value);
    }

    SVec4 operator/(SVec4 left, float value) {
        return left.Divide(value);
    }

    bool SVec4::operator==(const SVec4 &other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool SVec4::operator!=(const SVec4 &other) const {
        return !(*this == other);
    }

    SVec4 &SVec4::operator+=(const SVec4 &other) {
        return Add(other);
    }

    SVec4 &SVec4::operator-=(const SVec4 &other) {
        return Subtract(other);
    }

    SVec4 &SVec4::operator*=(const SVec4 &other) {
        return Multiply(other);
    }

    SVec4 &SVec4::operator/=(const SVec4 &other) {
        return Divide(other);
    }

    bool SVec4::operator<(const SVec4 &other) const {
        return x < other.x && y < other.y && z < other.z && w < other.w;
    }

    bool SVec4::operator<=(const SVec4 &other) const {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }

    bool SVec4::operator>(const SVec4 &other) const {
        return x > other.x && y > other.y && z > other.z && w > other.w;
    }

    bool SVec4::operator>=(const SVec4 &other) const {
        return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
    }

    float SVec4::Dot(const SVec4 &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

}