//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/mat3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mat3::Mat3() {
        std::memset(elements, 0, 9 * sizeof(float32));
    }

    //--------------------------------------------------------------
    Mat3::Mat3(float32 diagonal) {
        std::memset(elements, 0, 9 * sizeof(float32));
        elements[0 + 0 * 3] = diagonal;
        elements[1 + 1 * 3] = diagonal;
        elements[2 + 2 * 3] = diagonal;
    }

    //--------------------------------------------------------------
    Mat3::Mat3(float32 *elements) {
        std::memcpy(this->elements, elements, 9 * sizeof(float32));
    }

    //--------------------------------------------------------------
    Mat3::Mat3(const Vec3 &column0, const Vec3 &column1, const Vec3 &column2) {
        columns[0] = column0;
        columns[1] = column1;
        columns[2] = column2;
    }

    //--------------------------------------------------------------
    Mat3::Mat3(const Mat4 &mat4) {
        columns[0] = mat4.columns[0];
        columns[1] = mat4.columns[1];
        columns[2] = mat4.columns[2];
    }

    //--------------------------------------------------------------
    Vec3 Mat3::GetRow(int index) const {
        return Vec3(elements[index + 0 * 3], elements[index + 1 * 3], elements[index + 2 * 3]);
    }

    //--------------------------------------------------------------
    void Mat3::SetRow(int index, const Vec3 &row) {
        elements[index + 0 * 3] = row.x;
        elements[index + 1 * 3] = row.y;
        elements[index + 2 * 3] = row.z;
    }

    //--------------------------------------------------------------
    String Mat3::ToString() const {
        return StringUtils::Format("({:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f})\n({:.2f}, {:.2f}, {:.2f})\n",
            columns[0].x, columns[1].x, columns[2].x,
            columns[0].y, columns[1].y, columns[2].y,
            columns[0].z, columns[1].z, columns[2].z
        );
    }

}