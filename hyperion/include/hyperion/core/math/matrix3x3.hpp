//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  // Matrix is in column-major order!
  // They can therefore be indexed like that: [row + column * 3]
  // The elements are layed out in memory in the following way:
  // [  0  3  6  ]
  // [  1  4  7  ]
  // [  2  5  8  ]

  struct Matrix3x3 {
    union {
      float32 elements[9];

      struct {
        float32 m11;
        float32 m21;
        float32 m31;
        float32 m12;
        float32 m22;
        float32 m32;
        float32 m13;
        float32 m23;
        float32 m33;
      };

      Vector3 columns[3];

      struct {
        Vector4 column1;
        Vector4 column2;
        Vector4 column3;
      };
    };

    Matrix3x3();
    Matrix3x3(float32 diagonal);
    Matrix3x3(float32 *elements);
    Matrix3x3(const Vector3 &column0, const Vector3 &column1, const Vector3 &column2);
    Matrix3x3(const Matrix4x4 &matrix4x4);

    Vector3 GetRow(int32 index) const;
    void SetRow(int32 index, const Vector3 &row);

    String ToString() const;
  };

}
