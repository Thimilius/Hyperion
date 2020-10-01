#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/copyable.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/vertex_format.hpp"

namespace Hyperion::Rendering {

    enum class MeshTopology {
        Triangles,
        Lines,
        LineStrip,
        Points
    };

    struct MeshData {
        Vector<Vec3> positions;
        Vector<Vec3> normals;
        Vector<Vec2> uvs;

        Vector<u32> indices;
    };

    struct SubMesh {
        MeshTopology topology;

        u32 index_count;
        u32 index_offset;
        u32 vertex_offset;
    };

    class Mesh : public Asset, public ICopyable<Mesh> {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        Mesh *Copy() const override;

        inline const MeshData &GetMeshData() const { return m_mesh_data; }
        inline BoundingBox GetBounds() const { return m_bounds; }
        inline VertexArray *GetVertexArray() const { return m_vertex_array; }
        inline const Vector<SubMesh> &GetSubMeshes() const { return m_sub_meshes; }

        static Mesh *Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes);
    private:
        Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes);

        void RecalculateBounds();
    private:
        MeshData m_mesh_data;
        BoundingBox m_bounds;

        Vector<SubMesh> m_sub_meshes;

        VertexArray *m_vertex_array;
    };

}