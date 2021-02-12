//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/quaternion.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Quaternion::Quaternion() { }

    //--------------------------------------------------------------
    Quaternion::Quaternion(float32 x, float32 y, float32 z, float32 w)
        : x(x), y(y), z(z), w(w) { }

    //--------------------------------------------------------------
    Quaternion::Quaternion(Vec3 xyz, float32 w)
        : x(xyz.x), y(xyz.y), z(xyz.z), w(w) { }

    //--------------------------------------------------------------
    Vec3 Quaternion::ToEulerAngles() const {
        float32 x_value = Math::RadToDeg(Math::ATan2(2 * x * w - 2 * y * z, 1 - 2 * x * x - 2 * z * z));
        float32 y_value = Math::RadToDeg(Math::ATan2(2 * y * w - 2 * x * z, 1 - 2 * y * y - 2 * z * z));
        float32 z_value = Math::RadToDeg(Math::ASin(2 * x * y + 2 * z * w));
        return Vec3(x_value, y_value, z_value);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::Normalized() const {
        float32 length_sqr = (x * x);
        length_sqr += (y * y);
        length_sqr += (z * z);
        length_sqr += (w * w);

        float32 length_inverse = 1.0f / Math::Sqrt(length_sqr);
        return *this * length_inverse;
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::Inverted() const {
        return Quaternion(-x, -y, -z, w);
    }

    //--------------------------------------------------------------
    String Quaternion::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})", x, y, z, w);
    }

    //--------------------------------------------------------------
    bool Quaternion::operator==(const Quaternion &other) const {
        return x == other.x && y == other.y && z == other.z && w == other.z;
    }

    //--------------------------------------------------------------
    bool Quaternion::operator!=(const Quaternion &other) const {
        return !(*this == other);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator-() const {
        return Quaternion(-x, -y, -z, -w);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator+(const Quaternion &other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator-(const Quaternion &other) const {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator*(const Quaternion &other) const {
        return Quaternion(
            (((w * other.x) + (x * other.w)) + (y * other.z)) - (z * other.y),
            (((w * other.y) + (y * other.w)) + (z * other.x)) - (x * other.z),
            (((w * other.z) + (z * other.w)) + (x * other.y)) - (y * other.x),
            (((w * other.w) - (x * other.x)) - (y * other.y)) - (z * other.z)
        ).Normalized();
    }

    //--------------------------------------------------------------
    Vec3 Quaternion::operator*(const Vec3 &other) const {
        return Rotate(*this, other);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator*(float32 value) const {
        return Quaternion(x * value, y * value, z * value, w * value);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::operator/(float32 value) const {
        return Quaternion(x / value, y / value, z / value, w / value);
    }

    //--------------------------------------------------------------
    Vec3 Quaternion::Rotate(const Quaternion &quaternion, const Vec3 &vec) {
        float32 x = (((quaternion.w * vec.x) + (quaternion.y * vec.z)) - (quaternion.z * vec.y));
        float32 y = (((quaternion.w * vec.y) + (quaternion.z * vec.x)) - (quaternion.x * vec.z));
        float32 z = (((quaternion.w * vec.z) + (quaternion.x * vec.y)) - (quaternion.y * vec.x));
        float32 w = (((quaternion.x * vec.x) + (quaternion.y * vec.y)) + (quaternion.z * vec.z));
        return Vec3(
            ((((w * quaternion.x) + (x * quaternion.w)) - (y * quaternion.z)) + (z * quaternion.y)),
            ((((w * quaternion.y) + (y * quaternion.w)) - (z * quaternion.x)) + (x * quaternion.z)),
            ((((w * quaternion.z) + (z * quaternion.w)) - (x * quaternion.y)) + (y * quaternion.x))
        );
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::FromAxisAngle(const Vec3 &axis, float32 angle) {
        angle = Math::DegToRad(angle) * 0.5f;
        return Quaternion((axis * Math::Sin(angle)), Math::Cos(angle));
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::FromEulerAngles(const Vec3 &angles) {
        return FromEulerAngles(angles.x, angles.y, angles.z);
    }

    //--------------------------------------------------------------
    Quaternion Quaternion::FromEulerAngles(float32 x, float32 y, float32 z) {
        x = Math::DegToRad(x) * 0.5f;
        y = Math::DegToRad(y) * 0.5f;
        z = Math::DegToRad(z) * 0.5f;

        float32 cr = Math::Cos(x);
        float32 sr = Math::Sin(x);
        float32 cp = Math::Cos(y);
        float32 sp = Math::Sin(y);
        float32 cy = Math::Cos(z);
        float32 sy = Math::Sin(z);

        Quaternion result;
        result.x = cy * cp * sr - sy * sp * cr;
        result.y = sy * cp * sr + cy * sp * cr;
        result.z = sy * cp * cr - cy * sp * sr;
        result.w = cy * cp * cr + sy * sp * sr;

        return result;
    }

}