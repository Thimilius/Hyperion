#pragma once

#include "hyperion/core/math/vec3.hpp"

namespace Hyperion {

    // Matrix is in column-major order!
    // They can therefore be indexed like that: [row + column * 3]
    // The elements are layed out in memory in the following way:
    // [  0  3  6  ]
    // [  1  4  7  ]
    // [  2  5  8  ]

    struct Mat3 {
        union {
            f32 elements[9];
            Vec3 columns[3];
        };

        Mat3();
        Mat3(f32 diagonal);
        Mat3(f32 *elements);
        Mat3(const Vec3 &column0, const Vec3 &column1, const Vec3 &column2);
        Mat3(const Mat4 &mat4);

        Vec3 GetRow(int index) const;
        void SetRow(int index, const Vec3 &row);

        String ToString() const;
    };

}