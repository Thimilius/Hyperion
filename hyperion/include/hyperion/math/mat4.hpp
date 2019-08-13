#pragma once

#include "vec4.hpp"
#include "vec3.hpp"

namespace Hyperion::Math {

    struct SMat4 {
        union {
            float elements[16]; // Index with elements[column + row * 4]
            SVec4 columns[4];
        };

        SMat4();
        SMat4(float diagonal);
        SMat4(float *elements);
        SMat4(const SVec4 &column0, const SVec4 &column1, const SVec4 &column2, const SVec4 &column3);

        SMat4 &Multiply(const SMat4 &other);
        SVec3 Multiply(const SVec3 &other) const;
        SVec4 Multiply(const SVec4 &other) const;

        SMat4 &operator*=(const SMat4 &other);
        friend SMat4 operator*(SMat4 left, const SMat4 &right);
        friend SVec3 operator*(const SMat4 &left, const SVec3 &right);
        friend SVec4 operator*(const SMat4 &left, const SVec4 &right);

        SMat4 &Invert();
        SMat4 Transpose();

        SVec4 GetRow(int index) const;
        void SetRow(int index, const SVec4 &column);

        TString ToString() const;

        static SMat4 Identity();
        static SMat4 Translate(const SVec3 &position);
        static SMat4 Translate(float x, float y, float z);
        static SMat4 Rotate(const SVec3 &axis, float angle);
        static SMat4 Scale(const SVec3 &scale);
        static SMat4 Scale(float x, float y, float z);
        static SMat4 TRS(const SVec3 &position, const SVec3 &axis, float angle, const SVec3 &scale);

        static SMat4 Orthographic(float left, float right, float bottom, float top, float z_near, float z_far);
        static SMat4 Perspective(float fov, float aspectRatio, float z_near, float z_far);
        static SMat4 LookAt(const SVec3 &position, const SVec3 &direction, const SVec3 &up);
    };

}