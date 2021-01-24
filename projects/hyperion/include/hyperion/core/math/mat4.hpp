#pragma once

#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/quaternion.hpp"

namespace Hyperion {

    // Matrix is in column-major order!
    // They can therefore be indexed like that: [row + column * 4]
    // The elements are layed out in memory in the following way:
    // [  0  4  8 12 ]
    // [  1  5  9 13 ]
    // [  2  6 10 14 ]
    // [  3  7 11 15 ]

    struct Mat4 {
        union {
            float32 elements[16];
            Vec4 columns[4];
        };

        Mat4();
        Mat4(float32 diagonal);
        Mat4(float32 *elements);
        Mat4(const Vec4 &column0, const Vec4 &column1, const Vec4 &column2, const Vec4 &column3);

        Mat4 &Multiply(const Mat4 &other);
        Vec3 Multiply(const Vec3 &other) const;
        Vec4 Multiply(const Vec4 &other) const;

        Mat4 &operator*=(const Mat4 &other);

        Mat4 Inverted() const;
        Mat4 Transposed() const;

        Vec4 GetRow(int index) const;
        void SetRow(int index, const Vec4 &row);

        String ToString() const;

        static Mat4 Identity();
        static Mat4 Translate(const Vec3 &position);
        static Mat4 Translate(float32 x, float32 y, float32 z);
        static Mat4 Rotate(const Vec3 &axis, float32 angle);
        static Mat4 Rotate(const Quaternion &quaternion);
        static Mat4 Scale(const Vec3 &scale);
        static Mat4 Scale(float32 x, float32 y, float32 z);
        static Mat4 TRS(const Vec3 &position, const Quaternion &rotation, const Vec3 &scale);

        static Mat4 Orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 z_near, float32 z_far);
        static Mat4 Perspective(float32 fov, float32 aspect_ratio, float32 z_near, float32 z_far);
        static Mat4 LookAt(const Vec3 &from, const Vec3 &to, const Vec3 &up);

        friend Mat4 operator*(Mat4 left, const Mat4 &right);
        friend Vec3 operator*(const Mat4 &left, const Vec3 &right);
        friend Vec4 operator*(const Mat4 &left, const Vec4 &right);
    };

}