#include "hyppch.hpp"

#include "hyperion/core/math/mat4.hpp"

namespace Hyperion {

    Mat4::Mat4() {
        std::memset(elements, 0, 16 * sizeof(f32));
    }

    Mat4::Mat4(f32 diagonal) {
        std::memset(elements, 0, 16 * sizeof(f32));
        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    Mat4::Mat4(f32 *elements) {
        std::memcpy(this->elements, elements, 16 * sizeof(f32));
    }

    Mat4::Mat4(const Vec4 &column0, const Vec4 &column1, const Vec4 &column2, const Vec4 &column3) {
        columns[0] = column0;
        columns[1] = column1;
        columns[2] = column2;
        columns[3] = column3;
    }

    Mat4 &Mat4::Multiply(const Mat4 &other) {
        f32 data[16];
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                f32 sum = 0.0f;
                for (int e = 0; e < 4; e++) {
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                data[x + y * 4] = sum;
            }
        }
        std::memcpy(elements, data, 16 * sizeof(f32));
        return *this;
    }

    Vec3 Mat4::Multiply(const Vec3 &other) const {
        return other.Multiply(*this);
    }

    Vec4 Mat4::Multiply(const Vec4 &other) const {
        return other.Multiply(*this);
    }

    Mat4 &Mat4::operator*=(const Mat4 &other) {
        return Multiply(other);
    }

    Mat4 Mat4::Inverted() const {
        f32 temp[16];

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

        f32 determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
        determinant = 1.0f / determinant;

        Mat4 result;
        for (int i = 0; i < 4 * 4; i++) {
            result.elements[i] = temp[i] * determinant;
        }

        return result;
    }

    Mat4 Mat4::Transposed()const {
        return Mat4(
            Vec4(columns[0].x, columns[1].x, columns[2].x, columns[3].x),
            Vec4(columns[0].y, columns[1].y, columns[2].y, columns[3].y),
            Vec4(columns[0].z, columns[1].z, columns[2].z, columns[3].z),
            Vec4(columns[0].w, columns[1].w, columns[2].w, columns[3].w)
        );
    }


    Vec4 Mat4::GetRow(int index) const {
        return Vec4(elements[index + 0 * 4], elements[index + 1 * 4], elements[index + 2 * 4], elements[index + 3 * 4]);
    }

    void Mat4::SetRow(int index, const Vec4 &row) {
        elements[index + 0 * 4] = row.x;
        elements[index + 1 * 4] = row.y;
        elements[index + 2 * 4] = row.z;
        elements[index + 3 * 4] = row.w;
    }

    String Mat4::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n",
            columns[0].x, columns[1].x, columns[2].x, columns[3].x,
            columns[0].y, columns[1].y, columns[2].y, columns[3].y,
            columns[0].z, columns[1].z, columns[2].z, columns[3].z,
            columns[0].w, columns[1].w, columns[2].w, columns[3].w
        );
    }

    Mat4 Mat4::Identity() {
        return Mat4(1.0f);
    }

    Mat4 Mat4::Translate(const Vec3 &position) {
        return Translate(position.x, position.y, position.z);
    }

    Mat4 Mat4::Translate(f32 x, f32 y, f32 z) {
        Mat4 result(1.0f);

        result.elements[0 + 3 * 4] = x;
        result.elements[1 + 3 * 4] = y;
        result.elements[2 + 3 * 4] = z;

        return result;
    }

    Mat4 Mat4::Rotate(const Vec3 &axis, f32 angle) {
        Mat4 result(1.0f);

        f32 radians = Math::DegToRad(angle);
        f32 cos = Math::Cos(radians);
        f32 sin = Math::Sin(radians);
        f32 omc = 1.0f - cos;

        f32 x = axis.x;
        f32 y = axis.y;
        f32 z = axis.z;

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

    Mat4 Mat4::Rotate(const Quaternion &quaternion) {
        Mat4 result = Identity();

        f32 qx = quaternion.x;
        f32 qy = quaternion.y;
        f32 qz = quaternion.z;
        f32 qw = quaternion.w;
        f32 qx2 = (qx + qx);
        f32 qy2 = (qy + qy);
        f32 qz2 = (qz + qz);
        f32 qxqx2 = (qx * qx2);
        f32 qxqy2 = (qx * qy2);
        f32 qxqz2 = (qx * qz2);
        f32 qxqw2 = (qw * qx2);
        f32 qyqy2 = (qy * qy2);
        f32 qyqz2 = (qy * qz2);
        f32 qyqw2 = (qw * qy2);
        f32 qzqz2 = (qz * qz2);
        f32 qzqw2 = (qw * qz2);

        result.columns[0] = Vec4((1.0f - qyqy2) - qzqz2, qxqy2 + qzqw2, qxqz2 - qyqw2, 0.0f);
        result.columns[1] = Vec4(qxqy2 - qzqw2, (1.0f - qxqx2) - qzqz2, qyqz2 + qxqw2, 0.0f);
        result.columns[2] = Vec4(qxqz2 + qyqw2, qyqz2 - qxqw2, (1.0f - qxqx2) - qyqy2, 0.0f);

        return result;
    }

    Mat4 Mat4::Scale(const Vec3 &scale) {
        return Scale(scale.x, scale.y, scale.z);
    }

    Mat4 Mat4::Scale(f32 x, f32 y, f32 z) {
        Mat4 result(1.0f);

        result.elements[0 + 0 * 4] = x;
        result.elements[1 + 1 * 4] = y;
        result.elements[2 + 2 * 4] = z;

        return result;
    }

    Mat4 Mat4::TRS(const Vec3 &position, const Quaternion &rotation, const Vec3 &scale) {
        return Translate(position) * Rotate(rotation) * Scale(scale);
    }

    Mat4 Mat4::Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far) {
        Mat4 result(1.0f);

        result.elements[0 + 0 * 4] = 2.0f / (right - left);
        result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
        result.elements[2 + 2 * 4] = 2.0f / (z_near - z_far);

        result.elements[0 + 3 * 4] = -(right + left)   / (right - left);
        result.elements[1 + 3 * 4] = -(top + bottom)   / (top - bottom);
        result.elements[2 + 3 * 4] = -(z_near + z_far) / (z_far - z_near);

        return result;
    }

    Mat4 Mat4::Perspective(f32 fov, f32 aspect_ratio, f32 z_near, f32 z_far) {
        Mat4 result(0.0f);

        f32 t = Math::Tan(0.5f * Math::DegToRad(fov)) * z_near;
        f32 b = -t;
        f32 r = t * aspect_ratio;
        f32 l = -r;

        result.elements[0 + 0 * 4] = 2.0f * z_near / (r - l);
        result.elements[1 + 1 * 4] = 2.0f * z_near / (t - b);
        result.elements[0 + 2 * 4] = (r + l) / (r - l);
        result.elements[1 + 2 * 4] = (t + b) / (t - b);
        result.elements[2 + 2 * 4] = -(z_far + z_near) / (z_far - z_near);
        result.elements[3 + 2 * 4] = -1.0f;
        result.elements[2 + 3 * 4] = -(2 * z_far * z_near) / (z_far - z_near);

        return result;
    }

    Mat4 Mat4::LookAt(const Vec3 &from, const Vec3 &to, const Vec3 &up) {
        Mat4 result(1.0f);

        Vec3 f = (to - from).Normalized();
        Vec3 s = Vec3::Cross(f, up.Normalized()).Normalized();
        Vec3 u = Vec3::Cross(s, f).Normalized();

        result.elements[0 + 0 * 4] = s.x;
        result.elements[0 + 1 * 4] = s.y;
        result.elements[0 + 2 * 4] = s.z;

        result.elements[1 + 0 * 4] = u.x;
        result.elements[1 + 1 * 4] = u.y;
        result.elements[1 + 2 * 4] = u.z;

        result.elements[2 + 0 * 4] = -f.x;
        result.elements[2 + 1 * 4] = -f.y;
        result.elements[2 + 2 * 4] = -f.z;

        return result * Translate(Vec3(-from.x, -from.y, -from.z));
    }

    Mat4 operator*(Mat4 left, const Mat4 &right) {
        return left.Multiply(right);
    }

    Vec3 operator*(const Mat4 &left, const Vec3 &right) {
        return left.Multiply(right);
    }

    Vec4 operator*(const Mat4 &left, const Vec4 &right) {
        return left.Multiply(right);
    }

}