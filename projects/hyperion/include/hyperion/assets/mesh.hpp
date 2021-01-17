#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/mesh_descriptor.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"

namespace Hyperion {

    struct MeshData {
        Vector<Vec3> positions;
        Vector<Vec3> normals;
        Vector<Vec2> texture0;

        Vector<uint32> indices;
    };

    struct VertexMesh {
        Vec3 position;
        Vec3 normal;
        Vec2 texture0;
    };

    class Mesh : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        inline const MeshData &GetMeshData() const { return m_mesh_data; }
        inline BoundingBox GetBounds() const { return m_bounds; }
        inline const Vector<SubMesh> &GetSubMeshes() const { return m_sub_meshes; }
        
        static Mesh *Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes);
    private:
        Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes);
        ~Mesh() override;

        void RecalculateBounds();
    private:
        MeshData m_mesh_data;
        BoundingBox m_bounds;

        Vector<SubMesh> m_sub_meshes;
    };

}