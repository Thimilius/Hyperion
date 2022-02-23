//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/quaternion.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  // Matrix is in column-major order!
  // They can therefore be indexed like that: [row + column * 4]
  // The elements are layed out in memory in the following way:
  // [  0  4  8 12 ]
  // [  1  5  9 13 ]
  // [  2  6 10 14 ]
  // [  3  7 11 15 ]
  // Furthermore matrices are always right-handed with the y-axis as up.

  struct Matrix4x4 {
    union {
      float32 elements[16];
      struct {
        float32 m11;
        float32 m21;
        float32 m31;
        float32 m41;
        float32 m12;
        float32 m22;
        float32 m32;
        float32 m42;
        float32 m13;
        float32 m23;
        float32 m33;
        float32 m43;
        float32 m14;
        float32 m24;
        float32 m34;
        float32 m44;
      };

      Vector4 columns[4];
      struct {
        Vector4 column1;
        Vector4 column2;
        Vector4 column3;
        Vector4 column4;
      };
    };

    Matrix4x4();
    Matrix4x4(float32 diagonal);
    Matrix4x4(float32 *elements);
    Matrix4x4(const Vector4 &column0, const Vector4 &column1, const Vector4 &column2, const Vector4 &column3);

    Matrix4x4 &Multiply(const Matrix4x4 &other);
    Vector3 Multiply(const Vector3 &other) const;
    Vector4 Multiply(const Vector4 &other) const;

    Matrix4x4 &operator*=(const Matrix4x4 &other);

    Matrix4x4 Inverted() const;
    Matrix4x4 Transposed() const;

    Vector4 GetRow(int32 index) const;
    void SetRow(int32 index, const Vector4 &row);

    String ToString() const;

    static Matrix4x4 Identity();
    static Matrix4x4 Translate(const Vector3 &position);
    static Matrix4x4 Translate(float32 x, float32 y, float32 z);
    static Matrix4x4 Rotate(const Vector3 &axis, float32 angle);
    static Matrix4x4 Rotate(const Quaternion &quaternion);
    static Matrix4x4 Scale(const Vector3 &scale);
    static Matrix4x4 Scale(float32 x, float32 y, float32 z);
    static Matrix4x4 TRS(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale);

    static Matrix4x4 Orthographic(float32 left, float32 right, float32 bottom, float32 top, float32 z_near, float32 z_far);
    static Matrix4x4 Perspective(float32 fov, float32 aspect_ratio, float32 z_near, float32 z_far);
    static Matrix4x4 LookAt(const Vector3 &from, const Vector3 &to, const Vector3 &up);

    friend Matrix4x4 operator*(Matrix4x4 left, const Matrix4x4 &right);
    friend Vector3 operator*(const Matrix4x4 &left, const Vector3 &right);
    friend Vector4 operator*(const Matrix4x4 &left, const Vector4 &right);
  };

}