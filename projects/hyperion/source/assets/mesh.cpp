//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/mesh.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mesh::Mesh(AssetInfo info, const MeshData &data, const Array<Rendering::SubMesh> &sub_meshes) : Mesh(info) {
        SetData(data, sub_meshes);
    }

    //--------------------------------------------------------------
    void Mesh::SetData(const MeshData &data, const Array<Rendering::SubMesh> &sub_meshes) {
        Threading::ScopeLock lock(GetLocker());

        m_data = data;
        m_sub_meshes = sub_meshes;

        // The minimum a mesh must provide is positional data.
        HYP_ASSERT(data.positions.GetLength() > 0);
        bool8 has_normals = data.normals.GetLength() > 0;
        bool8 has_colors = data.colors.GetLength() > 0;
        bool8 has_texture0 = data.texture0.GetLength() > 0;

        m_vertex_format.attributes.Add({ VertexAttributeKind::Position, VertexAttributeType::Float32, 3 });
        uint32 stride = MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_POSITION;

        if (has_normals) {
            HYP_ASSERT(data.normals.GetLength() == data.positions.GetLength());
            m_vertex_format.attributes.Add({ VertexAttributeKind::Normal, VertexAttributeType::Float32, 3 });
            stride += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_NORMAL;
        }
        if (has_colors) {
            HYP_ASSERT(data.colors.GetLength() == data.positions.GetLength());
            m_vertex_format.attributes.Add({ VertexAttributeKind::Color, VertexAttributeType::Float32, 4 });
            stride += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_COLOR;
        }
        if (has_texture0) {
            HYP_ASSERT(data.texture0.GetLength() == data.positions.GetLength());
            m_vertex_format.attributes.Add({ VertexAttributeKind::Texture0, VertexAttributeType::Float32, 2 });
            stride += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
        }
        m_vertex_format.stride = stride;

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