//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/matrix3x3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Matrix3x3::Matrix3x3() {
    std::memset(elements, 0, 9 * sizeof(float32));
  }

  //--------------------------------------------------------------
  Matrix3x3::Matrix3x3(float32 diagonal) {
    std::memset(elements, 0, 9 * sizeof(float32));
    elements[0 + 0 * 3] = diagonal;
    elements[1 + 1 * 3] = diagonal;
    elements[2 + 2 * 3] = diagonal;
  }

  //--------------------------------------------------------------
  Matrix3x3::Matrix3x3(float32 *elements) {
    std::memcpy(this->elements, elements, 9 * sizeof(float32));
  }

  //--------------------------------------------------------------
  Matrix3x3::Matrix3x3(const Vector3 &column0, const Vector3 &column1, const Vector3 &column2) {
    columns[0] = column0;
    columns[1] = column1;
    columns[2] = column2;
  }

  //--------------------------------------------------------------
  Matrix3x3::Matrix3x3(const Matrix4x4 &matrix4x4) {
    columns[0] = matrix4x4.columns[0];
    columns[1] = matrix4x4.columns[1];
    columns[2] = matrix4x4.columns[2];
  }

  //--------------------------------------------------------------
  Vector3 Matrix3x3::GetRow(int32 index) const {
    return Vector3(elements[index + 0 * 3], elements[index + 1 * 3], elements[index + 2 * 3]);
  }

  //--------------------------------------------------------------
  void Matrix3x3::SetRow(int32 index, const Vector3 &row) {
    elements[index + 0 * 3] = row.x;
    elements[index + 1 * 3] = row.y;
    elements[index + 2 * 3] = row.z;
  }

  //--------------------------------------------------------------
  String Matrix3x3::ToString() const {
    return StringUtils::Format("({:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f})\n",
                               columns[0].x, columns[1].x, columns[2].x,
                               columns[0].y, columns[1].y, columns[2].y,
                               columns[0].z, columns[1].z, columns[2].z
    );
  }

}
