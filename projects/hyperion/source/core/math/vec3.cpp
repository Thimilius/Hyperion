#include "hyppch.hpp"

namespace Hyperion {

    Vec3::Vec3()
        : x(0), y(0), z(0) { }

    Vec3::Vec3(float32 x, float32 y, float32 z)
        : x(x), y(y), z(z) { }

    Vec3::Vec3(const Vec2 &vec2, float32 z)
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

    Vec3 &Vec3::Add(float32 value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    Vec3 &Vec3::Subtract(float32 value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    Vec3 &Vec3::Multiply(float32 value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3 &Vec3::Divide(float32 value) {
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

    Vec3 &Vec3::operator-() {
        x = -x;
        y = -y;
        z = -z;

        return *this;
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

    float32 Vec3::Dot(const Vec3 &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 Vec3::Normalized() const {
        float32 magnitude = Magnitude();
        if (magnitude == 0) {
            return Vec3();
        } else {
            return Vec3(x / magnitude, y / magnitude, z / magnitude);
        }
    }

    float32 Vec3::Magnitude() const {
        return Math::Sqrt(x * x + y * y + z * z);
    }

    float32 Vec3::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    float32 Vec3::Distance(const Vec3 &other) const {
        float32 x = this->x - other.x;
        float32 y = this->y - other.y;
        float32 z = this->z - other.z;

        return Math::Sqrt(x * x + y * y + z * z);
    }

    Vec3 Vec3::Cross(const Vec3 &a, const Vec3 &b) {
        float32 x = a.y * b.z - a.z * b.y;
        float32 y = a.z * b.x - a.x * b.z;
        float32 z = a.x * b.y - a.y * b.x;

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

    Vec3 operator+(Vec3 left, float32 right) {
        return left.Add(right);
    }

    Vec3 operator-(Vec3 left, float32 right) {
        return left.Subtract(right);
    }

    Vec3 operator*(Vec3 left, float32 right) {
        return left.Multiply(right);
    }

    Vec3 operator/(Vec3 left, float32 right) {
        return left.Divide(right);
    }

    Vec3 operator+(float32 left, Vec3 right) {
        return right.Add(left);
    }

    Vec3 operator-(float32 left, Vec3 right) {
        return right.Subtract(left);
    }

    Vec3 operator*(float32 left, Vec3 right) {
        return right.Multiply(left);
    }

    Vec3 operator/(float32 left, Vec3 right) {
        return right.Divide(left);
    }

    Vec3Int::Vec3Int()
        : x(0), y(0), z(0) { }

    Vec3Int::Vec3Int(int32 x, int32 y, int32 z)
        : x(x), y(y), z(z) { }

    Vec3Int::Vec3Int(const Vec2Int &vec2, int32 z)
        : x(vec2.x), y(vec2.y), z(z) { }

    Vec3Int &Vec3Int::Add(const Vec3Int &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vec3Int &Vec3Int::Subtract(const Vec3Int &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Vec3Int &Vec3Int::Multiply(const Vec3Int &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;

        return *this;
    }

    Vec3Int &Vec3Int::Divide(const Vec3Int &other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;

        return *this;
    }

    Vec3Int &Vec3Int::Add(int32 value) {
        x += value;
        y += value;
        z += value;

        return *this;
    }

    Vec3Int &Vec3Int::Subtract(int32 value) {
        x -= value;
        y -= value;
        z -= value;

        return *this;
    }

    Vec3Int &Vec3Int::Multiply(int32 value) {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }

    Vec3Int &Vec3Int::Divide(int32 value) {
        x /= value;
        y /= value;
        z /= value;

        return *this;
    }

    String Vec3Int::ToString() const {
        return StringUtils::Format("({}, {}, {})", x, y, z);
    }

    bool Vec3Int::operator==(const Vec3Int &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Vec3Int::operator!=(const Vec3Int &other) const {
        return !(*this == other);
    }

    Vec3Int &Vec3Int::operator-() {
        x = -x;
        y = -y;
        z = -z;

        return *this;
    }

    Vec3Int &Vec3Int::operator+=(const Vec3Int &other) {
        return Add(other);
    }

    Vec3Int &Vec3Int::operator-=(const Vec3Int &other) {
        return Subtract(other);
    }

    Vec3Int &Vec3Int::operator*=(const Vec3Int &other) {
        return Multiply(other);
    }

    Vec3Int &Vec3Int::operator/=(const Vec3Int &other) {
        return Divide(other);
    }

    bool Vec3Int::operator<(const Vec3Int &other) const {
        return x < other.x &&y < other.y &&z < other.z;
    }

    bool Vec3Int::operator<=(const Vec3Int &other) const {
        return x <= other.x && y <= other.y && z <= other.z;
    }

    bool Vec3Int::operator>(const Vec3Int &other) const {
        return x > other.x && y > other.y && z > other.z;
    }

    bool Vec3Int::operator>=(const Vec3Int &other) const {
        return x >= other.x && y >= other.y && z >= other.z;
    }

    float32 Vec3Int::Magnitude() const {
        float32 x = static_cast<float32>(this->x);
        float32 y = static_cast<float32>(this->y);
        float32 z = static_cast<float32>(this->z);

        return Math::Sqrt(x * x + y * y + z * z);
    }

    int32 Vec3Int::SqrMagnitude() const {
        return x * x + y * y + z * z;
    }

    float32 Vec3Int::Distance(const Vec3Int &other) const {
        float32 x = static_cast<float32>(this->x - other.x);
        float32 y = static_cast<float32>(this->y - other.y);
        float32 z = static_cast<float32>(this->z - other.z);

        return Math::Sqrt(x * x + y * y + z * z);
    }

    Vec3Int operator+(Vec3Int left, const Vec3Int &right) {
        return left.Add(right);
    }

    Vec3Int operator-(Vec3Int left, const Vec3Int &right) {
        return left.Subtract(right);
    }

    Vec3Int operator*(Vec3Int left, const Vec3Int &right) {
        return left.Multiply(right);
    }

    Vec3Int operator/(Vec3Int left, const Vec3Int &right) {
        return left.Divide(right);
    }

    Vec3Int operator+(Vec3Int left, int32 right) {
        return left.Add(right);
    }

    Vec3Int operator-(Vec3Int left, int32 right) {
        return left.Subtract(right);
    }

    Vec3Int operator*(Vec3Int left, int32 right) {
        return left.Multiply(right);
    }

    Vec3Int operator/(Vec3Int left, int32 right) {
        return left.Divide(right);
    }

    Vec3Int operator+(int32 left, Vec3Int right) {
        return right.Add(left);
    }

    Vec3Int operator-(int32 left, Vec3Int right) {
        return right.Subtract(left);
    }

    Vec3Int operator*(int32 left, Vec3Int right) {
        return right.Multiply(left);
    }

    Vec3Int operator/(int32 left, Vec3Int right) {
        return right.Divide(left);
    }
}