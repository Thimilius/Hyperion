//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Matrix4x4::Matrix4x4() {
        std::memset(elements, 0, 16 * sizeof(float32));
    }

    //--------------------------------------------------------------
    Matrix4x4::Matrix4x4(float32 diagonal) {
        std::memset(elements, 0, 16 * sizeof(float32));
        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    //--------------------------------------------------------------
    Matrix4x4::Matrix4x4(float32 *elements) {
        std::memcpy(this->elements, elements, 16 * sizeof(float32));
    }

    //--------------------------------------------------------------
    Matrix4x4::Matrix4x4(const Vector4 &column0, const Vector4 &column1, const Vector4 &column2, const Vector4 &column3) {
        columns[0] = column0;
        columns[1] = column1;
        columns[2] = column2;
        columns[3] = column3;
    }

    //--------------------------------------------------------------
    Matrix4x4 &Matrix4x4::Multiply(const Matrix4x4 &other) {
        float32 data[16];
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                float32 sum = 0.0f;
                for (int e = 0; e < 4; e++) {
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                data[x + y * 4] = sum;
            }
        }
        std::memcpy(elements, data, 16 * sizeof(float32));
        return *this;
    }

    //--------------------------------------------------------------
    Vector3 Matrix4x4::Multiply(const Vector3 &other) const {
        return other.Multiply(*this);
    }

    //--------------------------------------------------------------
    Vector4 Matrix4x4::Multiply(const Vector4 &other) const {
        return other.Multiply(*this);
    }

    //--------------------------------------------------------------
    Matrix4x4 &Matrix4x4::operator*=(const Matrix4x4 &other) {
        return Multiply(other);
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Inverted() const {
        float32 temp[16];

        temp[0] = elements[5] * elements[10] * elements[15] -
            elements[5] * elements[11] * elements[14] -
            elements[9] * elements[6] * elements[15] +
            elements[9] * elements[7] * elements[14] +
            elements[13] * elements[6] * elements[11] -
            elements[13] * elements[7] * elements[10];

        temp[4] = -elements[4] * elements[10] * elements[15] +
            elements[4] * elements[11] * elements[14] +
            elements[8] * elements[6] * elements[15] -
            elements[8] * elements[7] * elements[14] -
            elements[12] * elements[6] * elements[11] +
            elements[12] * elements[7] * elements[10];

        temp[8] = elements[4] * elements[9] * elements[15] -
            elements[4] * elements[11] * elements[13] -
            elements[8] * elements[5] * elements[15] +
            elements[8] * elements[7] * elements[13] +
            elements[12] * elements[5] * elements[11] -
            elements[12] * elements[7] * elements[9];

        temp[12] = -elements[4] * elements[9] * elements[14] +
            elements[4] * elements[10] * elements[13] +
            elements[8] * elements[5] * elements[14] -
            elements[8] * elements[6] * elements[13] -
            elements[12] * elements[5] * elements[10] +
            elements[12] * elements[6] * elements[9];

        temp[1] = -elements[1] * elements[10] * elements[15] +
            elements[1] * elements[11] * elements[14] +
            elements[9] * elements[2] * elements[15] -
            elements[9] * elements[3] * elements[14] -
            elements[13] * elements[2] * elements[11] +
            elements[13] * elements[3] * elements[10];

        temp[5] = elements[0] * elements[10] * elements[15] -
            elements[0] * elements[11] * elements[14] -
            elements[8] * elements[2] * elements[15] +
            elements[8] * elements[3] * elements[14] +
            elements[12] * elements[2] * elements[11] -
            elements[12] * elements[3] * elements[10];

        temp[9] = -elements[0] * elements[9] * elements[15] +
            elements[0] * elements[11] * elements[13] +
            elements[8] * elements[1] * elements[15] -
            elements[8] * elements[3] * elements[13] -
            elements[12] * elements[1] * elements[11] +
            elements[12] * elements[3] * elements[9];

        temp[13] = elements[0] * elements[9] * elements[14] -
            elements[0] * elements[10] * elements[13] -
            elements[8] * elements[1] * elements[14] +
            elements[8] * elements[2] * elements[13] +
            elements[12] * elements[1] * elements[10] -
            elements[12] * elements[2] * elements[9];

        temp[2] = elements[1] * elements[6] * elements[15] -
            elements[1] * elements[7] * elements[14] -
            elements[5] * elements[2] * elements[15] +
            elements[5] * elements[3] * elements[14] +
            elements[13] * elements[2] * elements[7] -
            elements[13] * elements[3] * elements[6];

        temp[6] = -elements[0] * elements[6] * elements[15] +
            elements[0] * elements[7] * elements[14] +
            elements[4] * elements[2] * elements[15] -
            elements[4] * elements[3] * elements[14] -
            elements[12] * elements[2] * elements[7] +
            elements[12] * elements[3] * elements[6];

        temp[10] = elements[0] * elements[5] * elements[15] -
            elements[0] * elements[7] * elements[13] -
            elements[4] * elements[1] * elements[15] +
            elements[4] * elements[3] * elements[13] +
            elements[12] * elements[1] * elements[7] -
            elements[12] * elements[3] * elements[5];

        temp[14] = -elements[0] * elements[5] * elements[14] +
            elements[0] * elements[6] * elements[13] +
            elements[4] * elements[1] * elements[14] -
            elements[4] * elements[2] * elements[13] -
            elements[12] * elements[1] * elements[6] +
            elements[12] * elements[2] * elements[5];

        temp[3] = -elements[1] * elements[6] * elements[11] +
            elements[1] * elements[7] * elements[10] +
            elements[5] * elements[2] * elements[11] -
            elements[5] * elements[3] * elements[10] -
            elements[9] * elements[2] * elements[7] +
            elements[9] * elements[3] * elements[6];

        temp[7] = elements[0] * elements[6] * elements[11] -
            elements[0] * elements[7] * elements[10] -
            elements[4] * elements[2] * elements[11] +
            elements[4] * elements[3] * elements[10] +
            elements[8] * elements[2] * elements[7] -
            elements[8] * elements[3] * elements[6];

        temp[11] = -elements[0] * elements[5] * elements[11] +
            elements[0] * elements[7] * elements[9] +
            elements[4] * elements[1] * elements[11] -
            elements[4] * elements[3] * elements[9] -
            elements[8] * elements[1] * elements[7] +
            elements[8] * elements[3] * elements[5];

        temp[15] = elements[0] * elements[5] * elements[10] -
            elements[0] * elements[6] * elements[9] -
            elements[4] * elements[1] * elements[10] +
            elements[4] * elements[2] * elements[9] +
            elements[8] * elements[1] * elements[6] -
            elements[8] * elements[2] * elements[5];

        float32 determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
        determinant = 1.0f / determinant;

        Matrix4x4 result;
        for (int i = 0; i < 4 * 4; i++) {
            result.elements[i] = temp[i] * determinant;
        }

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Transposed() const {
        return Matrix4x4(
            Vector4(columns[0].x, columns[1].x, columns[2].x, columns[3].x),
            Vector4(columns[0].y, columns[1].y, columns[2].y, columns[3].y),
            Vector4(columns[0].z, columns[1].z, columns[2].z, columns[3].z),
            Vector4(columns[0].w, columns[1].w, columns[2].w, columns[3].w)
        );
    }


    //--------------------------------------------------------------
    Vector4 Matrix4x4::GetRow(int32 index) const {
        return Vector4(elements[index + 0 * 4], elements[index + 1 * 4], elements[index + 2 * 4], elements[index + 3 * 4]);
    }

    //--------------------------------------------------------------
    void Matrix4x4::SetRow(int32 index, const Vector4 &row) {
        elements[index + 0 * 4] = row.x;
        elements[index + 1 * 4] = row.y;
        elements[index + 2 * 4] = row.z;
        elements[index + 3 * 4] = row.w;
    }

    //--------------------------------------------------------------
    String Matrix4x4::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n",
            columns[0].x, columns[1].x, columns[2].x, columns[3].x,
            columns[0].y, columns[1].y, columns[2].y, columns[3].y,
            columns[0].z, columns[1].z, columns[2].z, columns[3].z,
            columns[0].w, columns[1].w, columns[2].w, columns[3].w
        );
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Identity() {
        return Matrix4x4(1.0f);
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Translate(const Vector3 &position) {
        return Translate(position.x, position.y, position.z);
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Translate(float32 x, float32 y, float32 z) {
        Matrix4x4 result(1.0f);

        result.elements[0 + 3 * 4] = x;
        result.elements[1 + 3 * 4] = y;
        result.elements[2 + 3 * 4] = z;

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Rotate(const Vector3 &axis, float32 angle) {
        Matrix4x4 result(1.0f);

        float32 radians = Math::DegToRad(angle);
        float32 cos = Math::Cos(radians);
        float32 sin = Math::Sin(radians);
        float32 omc = 1.0f - cos;

        float32 x = axis.x;
        float32 y = axis.y;
        float32 z = axis.z;

        result.elements[0 + 0 * 4] = x * x * omc + cos;
        result.elements[1 + 0 * 4] = y * x * omc + z * sin;
        result.elements[2 + 0 * 4] = x * z * omc - y * sin;

        result.elements[0 + 1 * 4] = x * y * omc - z * sin;
        result.elements[1 + 1 * 4] = y * y * omc + cos;
        result.elements[2 + 1 * 4] = y * z * omc + x * sin;

        result.elements[0 + 2 * 4] = x * z * omc + y * sin;
        result.elements[1 + 2 * 4] = y * z * omc - x * sin;
        result.elements[2 + 2 * 4] = z * z * omc + cos;

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Rotate(const Quaternion &quaternion) {
        Matrix4x4 result = Identity();

        float32 qx = quaternion.x;
        float32 qy = quaternion.y;
        float32 qz = quaternion.z;
        float32 qw = quaternion.w;
        float32 qx2 = (qx + qx);
        float32 qy2 = (qy + qy);
        float32 qz2 = (qz + qz);
        float32 qxqx2 = (qx * qx2);
        float32 qxqy2 = (qx * qy2);
        float32 qxqz2 = (qx * qz2);
        float32 qxqw2 = (qw * qx2);
        float32 qyqy2 = (qy * qy2);
        float32 qyqz2 = (qy * qz2);
        float32 qyqw2 = (qw * qy2);
        float32 qzqz2 = (qz * qz2);
        float32 qzqw2 = (qw * qz2);

        result.columns[0] = Vector4((1.0f - qyqy2) - qzqz2, qxqy2 + qzqw2, qxqz2 - qyqw2, 0.0f);
        result.columns[1] = Vector4(qxqy2 - qzqw2, (1.0f - qxqx2) - qzqz2, qyqz2 + qxqw2, 0.0f);
        result.columns[2] = Vector4(qxqz2 + qyqw2, qyqz2 - qxqw2, (1.0f - qxqx2) - qyqy2, 0.0f);

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Scale(const Vector3 &scale) {
        return Scale(scale.x, scale.y, scale.z);
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Scale(float32 x, float32 y, float32 z) {
        Matrix4x4 result(1.0f);

        result.elements[0 + 0 * 4] = x;
        result.elements[1 + 1 * 4] = y;
        result.elements[2 + 2 * 4] = z;

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::TRS(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale) {
        Matrix4x4 result;

        result.elements[0 + 0 * 4] = (1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z)) * scale.x;
        result.elements[1 + 0 * 4] = (rotation.x * rotation.y + rotation.z * rotation.w) * scale.x * 2.0f;
        result.elements[2 + 0 * 4] = (rotation.x * rotation.z - rotation.y * rotation.w) * scale.x * 2.0f;
        result.elements[3 + 0 * 4] = 0.0f;
        result.elements[0 + 1 * 4] = (rotation.x * rotation.y - rotation.z * rotation.w) * scale.y * 2.0f;
        result.elements[1 + 1 * 4] = (1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z)) * scale.y;
        result.elements[2 + 1 * 4] = (rotation.y * rotation.z + rotation.x * rotation.w) * scale.y * 2.0f;
        result.elements[3 + 1 * 4] = 0.0f;
        result.elements[0 + 2 * 4] = (rotation.x * rotation.z + rotation.y * rotation.w) * scale.z * 2.0f;
        result.elements[1 + 2 * 4] = (rotation.y * rotation.z - rotation.x * rotation.w) * scale.z * 2.0f;
        result.elements[2 + 2 * 4] = (1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y)) * scale.z;
        result.elements[3 + 2 * 4] = 0.0f;
        result.elements[0 + 3 * 4] = position.x;
        result.elements[1 + 3 * 4] = position.y;
        result.elements[2 + 3 * 4] = position.z;
        result.elements[3 + 3 * 4] = 1.0f;

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 z_near, float32 z_far) {
        Matrix4x4 result(1.0f);

        result.elements[0 + 0 * 4] = 2.0f / (right - left);
        result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
        result.elements[2 + 2 * 4] = 2.0f / (z_near - z_far);

        result.elements[0 + 3 * 4] = -(right + left)   / (right - left);
        result.elements[1 + 3 * 4] = -(top + bottom)   / (top - bottom);
        result.elements[2 + 3 * 4] = -(z_near + z_far) / (z_far - z_near);

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::Perspective(float32 fov, float32 aspect_ratio, float32 z_near, float32 z_far) {
        Matrix4x4 result(0.0f);

        float32 t = Math::Tan(0.5f * Math::DegToRad(fov)) * z_near;
        float32 b = -t;
        float32 r = t * aspect_ratio;
        float32 l = -r;

        result.elements[0 + 0 * 4] = 2.0f * z_near / (r - l);
        result.elements[1 + 1 * 4] = 2.0f * z_near / (t - b);
        result.elements[0 + 2 * 4] = (r + l) / (r - l);
        result.elements[1 + 2 * 4] = (t + b) / (t - b);
        result.elements[2 + 2 * 4] = -(z_far + z_near) / (z_far - z_near);
        result.elements[3 + 2 * 4] = -1.0f;
        result.elements[2 + 3 * 4] = -(2 * z_far * z_near) / (z_far - z_near);

        return result;
    }

    //--------------------------------------------------------------
    Matrix4x4 Matrix4x4::LookAt(const Vector3 &from, const Vector3 &to, const Vector3 &up) {
        Matrix4x4 result(1.0f);

        Vector3 f = (to - from).Normalized();
        Vector3 s = Vector3::Cross(f, up.Normalized()).Normalized();
        Vector3 u = Vector3::Cross(s, f).Normalized();

        result.elements[0 + 0 * 4] = s.x;
        result.elements[0 + 1 * 4] = s.y;
        result.elements[0 + 2 * 4] = s.z;

        result.elements[1 + 0 * 4] = u.x;
        result.elements[1 + 1 * 4] = u.y;
        result.elements[1 + 2 * 4] = u.z;

        result.elements[2 + 0 * 4] = -f.x;
        result.elements[2 + 1 * 4] = -f.y;
        result.elements[2 + 2 * 4] = -f.z;

        return result * Translate(Vector3(-from.x, -from.y, -from.z));
    }

    //--------------------------------------------------------------
    Matrix4x4 operator*(Matrix4x4 left, const Matrix4x4 &right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector3 operator*(const Matrix4x4 &left, const Vector3 &right) {
        return left.Multiply(right);
    }

    //--------------------------------------------------------------
    Vector4 operator*(const Matrix4x4 &left, const Vector4 &right) {
        return left.Multiply(right);
    }

}