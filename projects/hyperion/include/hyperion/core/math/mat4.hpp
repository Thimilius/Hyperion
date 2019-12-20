#pragma once

#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/vec3.hpp"

namespace Hyperion {

    struct Mat4 {
        union {
            float elements[16]; // Index with elements[row + column * 4]
            Vec4 columns[4];
        };

        Mat4();
        Mat4(float diagonal);
        Mat4(float *elements);
        Mat4(const Vec4 &column0, const Vec4 &column1, const Vec4 &column2, const Vec4 &column3);

        Mat4 &Multiply(const Mat4 &other);
        Vec3 Multiply(const Vec3 &other) const;
        Vec4 Multiply(const Vec4 &other) const;

        Mat4 &operator*=(const Mat4 &other);
        friend Mat4 operator*(Mat4 left, const Mat4 &right);
        friend Vec3 operator*(const Mat4 &left, const Vec3 &right);
        friend Vec4 operator*(const Mat4 &left, const Vec4 &right);

        Mat4 &Invert();
        Mat4 Transpose();

        Vec4 GetRow(int index) const;
        void SetRow(int index, const Vec4 &column);

        String ToString() const;

        static Mat4 Identity();
        static Mat4 Translate(const Vec3 &position);
        static Mat4 Translate(float x, float y, float z);
        static Mat4 Rotate(const Vec3 &axis, float angle);
        static Mat4 Scale(const Vec3 &scale);
        static Mat4 Scale(float x, float y, float z);
        static Mat4 TRS(const Vec3 &position, const Vec3 &axis, float angle, const Vec3 &scale);

        static Mat4 Orthographic(float left, float right, float bottom, float top, float z_near, float z_far);
        static Mat4 Perspective(float fov, float aspect_ratio, float z_near, float z_far);
        static Mat4 LookAt(const Vec3 &from, const Vec3 &to, const Vec3 &up);
    };

}