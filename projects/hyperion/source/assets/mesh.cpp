//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mesh::Mesh(AssetInfo info, const MeshData &data) : Mesh(info) {
        m_data = data;
        m_bounds = CalculateBounds(data.positions);

        SetDirty();
    }

    //--------------------------------------------------------------
    BoundingBox Mesh::CalculateBounds(const Array<Vector3> &positions) {
        Vector3 min = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3 max = Vector3(FLT_MIN, FLT_MIN, FLT_MIN);

        for (uint32 i = 0; i < positions.GetLength(); i++) {
            Vector3 position = positions[i];
            if (position.x < min.x) {
                min.x = position.x;
            }
            if (position.y < min.y) {
                min.y = position.y;
            }
            if (position.z < min.z) {
                min.z = position.z;
            }
            if (position.x > max.x) {
                max.x = position.x;
            }
            if (position.y > max.y) {
                max.y = position.y;
            }
            if (position.z > max.z) {
                max.z = position.z;
            }
        }

        return BoundingBox(min, max);
    }

}