#include "hyppch.hpp"

#include "hyperion/math/mat4.hpp"

namespace Hyperion::Math {

    SMat4::SMat4() {
        memset(elements, 0, 16 * sizeof(float));
    }

    SMat4::SMat4(float diagonal) {
        memset(elements, 0, 16 * sizeof(float));
        elements[0 + 0 * 4] = diagonal;
        elements[1 + 1 * 4] = diagonal;
        elements[2 + 2 * 4] = diagonal;
        elements[3 + 3 * 4] = diagonal;
    }

    SMat4::SMat4(float *elements) {
        memcpy(this->elements, elements, 16 * sizeof(float));
    }

    SMat4::SMat4(const SVec4 &column0, const SVec4 &column1, const SVec4 &column2, const SVec4 &column3) {
        columns[0] = column0;
        columns[1] = column1;
        columns[2] = column2;
        columns[3] = column3;
    }

    SMat4 &SMat4::Multiply(const SMat4 &other) {
        float data[16];
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                float sum = 0.0f;
                for (int e = 0; e < 4; e++) {
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                data[x + y * 4] = sum;
            }
        }
        memcpy(elements, data, 16 * sizeof(float));
        return *this;
    }

    SVec3 SMat4::Multiply(const SVec3 &other) const {
        return other.Multiply(*this);
    }

    SVec4 SMat4::Multiply(const SVec4 &other) const {
        return other.Multiply(*this);
    }

    SMat4 operator*(SMat4 left, const SMat4 &right) {
        return left.Multiply(right);
    }

    SMat4 &SMat4::operator*=(const SMat4 &other) {
        return Multiply(other);
    }

    SVec3 operator*(const SMat4 &left, const SVec3 &right) {
        return left.Multiply(right);
    }

    SVec4 operator*(const SMat4 &left, const SVec4 &right) {
        return left.Multiply(right);
    }

    SMat4 &SMat4::Invert() {
        float temp[16];

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

        float determinant = elements[0] * temp[0] + elements[1] * temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
        determinant = 1.0f / determinant;

        for (int i = 0; i < 4 * 4; i++) {
            elements[i] = temp[i] * determinant;
        }

        return *this;
    }

    SMat4 SMat4::Transpose() {
        return SMat4(
            SVec4(columns[0].x, columns[1].x, columns[2].x, columns[3].x),
            SVec4(columns[0].y, columns[1].y, columns[2].y, columns[3].y),
            SVec4(columns[0].z, columns[1].z, columns[2].z, columns[3].z),
            SVec4(columns[0].w, columns[1].w, columns[2].w, columns[3].w)
        );
    }


    SVec4 SMat4::GetRow(int index) const {
        return SVec4(elements[index + 0 * 4], elements[index + 1 * 4], elements[index + 2 * 4], elements[index + 3 * 4]);
    }

    void SMat4::SetRow(int index, const SVec4 &column) {
        elements[index + 0 * 4] = column.x;
        elements[index + 1 * 4] = column.y;
        elements[index + 2 * 4] = column.z;
        elements[index + 3 * 4] = column.w;
    }

    CString SMat4::ToString() const {
        return CStringUtils::Format("({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f}, {:.2f})\n",
            columns[0].x, columns[1].x, columns[2].x, columns[3].x,
            columns[0].y, columns[1].y, columns[2].y, columns[3].y,
            columns[0].z, columns[1].z, columns[2].z, columns[3].z,
            columns[0].w, columns[1].w, columns[2].w, columns[3].w
        );
    }

    SMat4 SMat4::Identity() {
        return SMat4(1.0f);
    }

    SMat4 SMat4::Translate(const SVec3 &position) {
        return Translate(position.x, position.y, position.z);
    }

    SMat4 SMat4::Translate(float x, float y, float z) {
        SMat4 result(1.0f);

        result.elements[0 + 3 * 4] = x;
        result.elements[1 + 3 * 4] = y;
        result.elements[2 + 3 * 4] = z;

        return result;
    }

    SMat4 SMat4::Rotate(const SVec3 &axis, float angle) {
        SMat4 result(1.0f);

        float radians = CMathf::ToRadians(angle);
        float cos = CMathf::Cos(radians);
        float sin = CMathf::Sin(radians);
        float omc = 1.0f - cos;

        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

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

    SMat4 SMat4::Scale(const SVec3 &scale) {
        return Scale(scale.x, scale.y, scale.z);
    }

    SMat4 SMat4::Scale(float x, float y, float z) {
        SMat4 result(1.0f);

        result.elements[0 + 0 * 4] = x;
        result.elements[1 + 1 * 4] = z;
        result.elements[2 + 2 * 4] = y;

        return result;
    }

    SMat4 SMat4::TRS(const SVec3 &position, const SVec3 &axis, float angle, const SVec3 &scale) {
        return Translate(position) * Rotate(axis, angle) * Scale(scale);
    }

    SMat4 SMat4::Orthographic(float left, float right, float bottom, float top, float z_near, float z_far) {
        SMat4 result(1.0f);

        result.elements[0 + 0 * 4] = 2.0f / (right - left);
        result.elements[1 + 1 * 4] = 2.0f / (top - bottom);
        result.elements[2 + 2 * 4] = 2.0f / (z_near - z_far);

        result.elements[0 + 3 * 4] = (left + right) / (left - right);
        result.elements[1 + 3 * 4] = (bottom + top) / (bottom - top);
        result.elements[2 + 3 * 4] = (z_far + z_near) / (z_far - z_near);

        return result;
    }

    SMat4 SMat4::Perspective(float fov, float aspectRatio, float z_near, float z_far) {
        SMat4 result(1.0f);

        float q = 1.0f / CMathf::Tan(CMathf::ToRadians(0.5f * fov));
        float a = q / aspectRatio;

        float b = (z_near + z_far) / (z_near - z_far);
        float c = (2.0f * z_near * z_far) / (z_near - z_far);

        result.elements[0 + 0 * 4] = a;
        result.elements[1 + 1 * 4] = q;
        result.elements[2 + 2 * 4] = b;
        result.elements[3 + 2 * 4] = -1.0f;
        result.elements[2 + 3 * 4] = c;

        return result;
    }

    SMat4 SMat4::LookAt(const SVec3 &camera, const SVec3 &object, const SVec3 &up) {
        SMat4 result(1.0f);
        SVec3 f = (object - camera).Normalize();
        SVec3 s = f.Cross(up.Normalize());
        SVec3 u = s.Cross(f);

        result.elements[0 + 0 * 4] = s.x;
        result.elements[1 + 0 * 4] = s.y;
        result.elements[2 + 0 * 4] = s.z;

        result.elements[0 + 1 * 4] = u.x;
        result.elements[1 + 1 * 4] = u.y;
        result.elements[2 + 1 * 4] = u.z;

        result.elements[0 + 2 * 4] = -f.x;
        result.elements[1 + 2 * 4] = -f.y;
        result.elements[2 + 2 * 4] = -f.z;

        return result * Translate(SVec3(-camera.x, -camera.y, -camera.z));
    }

}