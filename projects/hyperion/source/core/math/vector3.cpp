//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Vector3::Vector3()
        : x(0), y(0), z(0) { }

    //--------------------------------------------------------------
    Vector3::Vector3(float32 x, float32 y, float32 z)
        : x(x), y(y), z(z) { }

    //--------------------------------------------------------------
    Vector3::Vector3(const Vector2 &vec2, float32 z)
        : x(vec2.x), y(vec2.y), z(z) { }

    //--------------------------------------------------------------
    Vector3::Vector3(const Vector4 &vec4)
        : x(vec4.x), y(vec4.y), z(vec4.z) { }

    //--------------------------------------------------------------
    Vector3 &Vector3::Add(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Subtract(const Vector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Multiply(const Vector3 &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Divide(const Vector3 &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Add(float32 value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Subtract(float32 value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Multiply(float32 value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::Divide(float32 value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 Vector3::Multiply(const Matrix4x4 &transform) const {
        return Vector3(
            transform.columns[0].x * x + transform.columns[1].x * y + transform.columns[2].x * z + transform.columns[3].x,
            transform.columns[0].y * x + transform.columns[1].y * y + transform.columns[2].y * z + transform.columns[3].y,
            transform.columns[0].z * x + transform.columns[1].z * y + transform.columns[2].z * z + transform.columns[3].z
        );
    }

    //--------------------------------------------------------------
    String Vector3::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f})", x, y, z);
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator==(const Vector3 &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator!=(const Vector3 &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::operator-() {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::operator+=(const Vector3 &other) {
        return Add(other);
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::operator-=(const Vector3 &other) {
        return Subtract(other);
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::operator*=(const Vector3 &other) {
        return Multiply(other);
    }

    //--------------------------------------------------------------
    Vector3 &Vector3::operator/=(const Vector3 &other) {
        return Divide(other);
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator<(const Vector3 &other) const {
        return x < other.x && y < other.y && z < other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator<=(const Vector3 &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator>(const Vector3 &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3::operator>=(const Vector3 &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    //--------------------------------------------------------------
    float32 Vector3::Dot(const Vector3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    //--------------------------------------------------------------
    Vector3 Vector3::Normalized() const {
        float32 magnitude = Magnitude();
        if (magnitude == 0) {
            return Vector3();
        } else {
            return Vector3(x / magnitude, y / magnitude, z / magnitude);
        }
    }

    //--------------------------------------------------------------
    float32 Vector3::Magnitude() const {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    //--------------------------------------------------------------
    float32 Vector3::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    //--------------------------------------------------------------
    float32 Vector3::Distance(const Vector3 &other) const {
        float32 x = this->x - other.x;
        float32 y = this->y - other.y;
        float32 z = this->z - other.z;

        return Math::Sqrt(x * x + y * y + z * z);
    }

    //--------------------------------------------------------------
    Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b) {
        float32 x = a.y * b.z - a.z * b.y;
        float32 y = a.z * b.x - a.x * b.z;
        float32 z = a.x * b.y - a.y * b.x;

        return Vector3(x, y, z);
    }

    //--------------------------------------------------------------
    Vector3 operator+(Vector3 left, const Vector3 &right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector3 operator-(Vector3 left, const Vector3 &right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector3 operator*(Vector3 left, const Vector3 &right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector3 operator/(Vector3 left, const Vector3 &right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector3 operator+(Vector3 left, float32 right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector3 operator-(Vector3 left, float32 right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector3 operator*(Vector3 left, float32 right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector3 operator/(Vector3 left, float32 right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector3 operator+(float32 left, Vector3 right) {
        return right.Add(left);
    }

    //--------------------------------------------------------------
    Vector3 operator-(float32 left, Vector3 right) {
        return right.Subtract(left);
    }

    //--------------------------------------------------------------
    Vector3 operator*(float32 left, Vector3 right) {
        return right.Multiply(left);
    }

    //--------------------------------------------------------------
    Vector3 operator/(float32 left, Vector3 right) {
        return right.Divide(left);
    }

    //--------------------------------------------------------------
    Vector3Int::Vector3Int()
        : x(0), y(0), z(0) { }

    //--------------------------------------------------------------
    Vector3Int::Vector3Int(int32 x, int32 y, int32 z)
        : x(x), y(y), z(z) { }

    //--------------------------------------------------------------
    Vector3Int::Vector3Int(const Vector2Int &vec2, int32 z)
        : x(vec2.x), y(vec2.y), z(z) { }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Add(const Vector3Int &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Subtract(const Vector3Int &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Multiply(const Vector3Int &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Divide(const Vector3Int &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Add(int32 value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Subtract(int32 value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Multiply(int32 value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::Divide(int32 value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    //--------------------------------------------------------------
    String Vector3Int::ToString() const {
        return StringUtils::Format("({}, {}, {})", x, y, z);
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator==(const Vector3Int &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator!=(const Vector3Int &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::operator-() {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::operator+=(const Vector3Int &other) {
        return Add(other);
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::operator-=(const Vector3Int &other) {
        return Subtract(other);
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::operator*=(const Vector3Int &other) {
        return Multiply(other);
    }

    //--------------------------------------------------------------
    Vector3Int &Vector3Int::operator/=(const Vector3Int &other) {
        return Divide(other);
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator<(const Vector3Int &other) const {
        return x < other.x &&y < other.y &&z < other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator<=(const Vector3Int &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator>(const Vector3Int &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    //--------------------------------------------------------------
    bool8 Vector3Int::operator>=(const Vector3Int &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    //--------------------------------------------------------------
    float32 Vector3Int::Magnitude() const {
        float32 x = static_cast<float32>(this->x);
        float32 y = static_cast<float32>(this->y);
        float32 z = static_cast<float32>(this->z);

        return Math::Sqrt(x * x + y * y + z * z);
    }

    //--------------------------------------------------------------
    int32 Vector3Int::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    //--------------------------------------------------------------
    float32 Vector3Int::Distance(const Vector3Int &other) const {
        float32 x = static_cast<float32>(this->x - other.x);
        float32 y = static_cast<float32>(this->y - other.y);
        float32 z = static_cast<float32>(this->z - other.z);

        return Math::Sqrt(x * x + y * y + z * z);
    }

    //--------------------------------------------------------------
    Vector3Int operator+(Vector3Int left, const Vector3Int &right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator-(Vector3Int left, const Vector3Int &right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator*(Vector3Int left, const Vector3Int &right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator/(Vector3Int left, const Vector3Int &right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator+(Vector3Int left, int32 right) {
        return left.Add(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator-(Vector3Int left, int32 right) {
        return left.Subtract(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator*(Vector3Int left, int32 right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator/(Vector3Int left, int32 right) {
        return left.Divide(right);
    }

    //--------------------------------------------------------------
    Vector3Int operator+(int32 left, Vector3Int right) {
        return right.Add(left);
    }

    //--------------------------------------------------------------
    Vector3Int operator-(int32 left, Vector3Int right) {
        return right.Subtract(left);
    }

    //--------------------------------------------------------------
    Vector3Int operator*(int32 left, Vector3Int right) {
        return right.Multiply(left);
    }

    //--------------------------------------------------------------
    Vector3Int operator/(int32 left, Vector3Int right) {
        return right.Divide(left);
    }
}