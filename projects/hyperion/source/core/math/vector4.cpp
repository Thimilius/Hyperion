//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/vector4.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Vector4::Vector4()
        : x(0), y(0), z(0), w(0) { }

    //--------------------------------------------------------------
    Vector4::Vector4(float32 x, float32 y, float32 z, float32 w)
        : x(x), y(y), z(z), w(w) { }

    //--------------------------------------------------------------
    Vector4::Vector4(const Vector3 &vec3, float32 w)
        : x(vec3.x), y(vec3.y), z(vec3.z), w(w) { }

    //--------------------------------------------------------------
    Vector4 &Vector4::Add(const Vector4 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Subtract(const Vector4 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Multiply(const Vector4 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Divide(const Vector4 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Add(float32 value) {
        x += value;
        y += value;
        z += value;
        w += value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Subtract(float32 value) {
        x -= value;
        y -= value;
        z -= value;
        w -= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Multiply(float32 value) {
        x *= value;
        y *= value;
        z *= value;
        w *= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::Divide(float32 value) {
        x /= value;
        y /= value;
        z /= value;
        w /= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 Vector4::Multiply(const Matrix4x4 &transform) const {
        return Vector4(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x * w,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y * w,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z * w,
            transform.columns[0].w * x + transform.columns[1].w * y + transform.columns[2].w * z + transform.columns[3].w * w
        );
    }

    //--------------------------------------------------------------
    String Vector4::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})", x, y, z, w);
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator==(const Vector4 &other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator!=(const Vector4 &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::operator-() {
        x = -x;
        y = -y;
        z = -z;
        w = -w;

        return *this;
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::operator+=(const Vector4 &other) {
        return Add(other);
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::operator-=(const Vector4 &other) {
        return Subtract(other);
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::operator*=(const Vector4 &other) {
        return Multiply(other);
    }

    //--------------------------------------------------------------
    Vector4 &Vector4::operator/=(const Vector4 &other) {
        return Divide(other);
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator<(const Vector4 &other) const {
        return x < other.x && y < other.y && z < other.z && w < other.w;
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator<=(const Vector4 &other) const {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator>(const Vector4 &other) const {
        return x > other.x && y > other.y && z > other.z && w > other.w;
    }

    //--------------------------------------------------------------
    bool8 Vector4::operator>=(const Vector4 &other) const {
        return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
    }

    //--------------------------------------------------------------
    Vector4::operator Color() {
        return Color(x, y, z, w);
    }

    //--------------------------------------------------------------
    float32 Vector4::Dot(const Vector4 &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    //--------------------------------------------------------------
    Vector4 operator+(Vector4 left, const Vector4 &right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector4 operator-(Vector4 left, const Vector4 &right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector4 operator*(Vector4 left, const Vector4 &right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector4 operator/(Vector4 left, const Vector4 &right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector4 operator+(Vector4 left, float32 right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector4 operator-(Vector4 left, float32 right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector4 operator*(Vector4 left, float32 right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector4 operator/(Vector4 left, float32 right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector4 operator+(float32 left, Vector4 right) {
        return right.Add(left);
    }

    //--------------------------------------------------------------
    Vector4 operator-(float32 left, Vector4 right) {
        return right.Subtract(left);
    }

    //--------------------------------------------------------------
    Vector4 operator*(float32 left, Vector4 right) {
        return right.Multiply(left);
    }

    //--------------------------------------------------------------
    Vector4 operator/(float32 left, Vector4 right) {
        return right.Divide(left);
    }

}