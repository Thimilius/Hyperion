#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/rendering/descriptors/mesh_descriptor.hpp"

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
        inline const Vector<Rendering::SubMesh> &GetSubMeshes() const { return m_sub_meshes; }
        
        static Mesh *Create(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes);
    private:
        Mesh(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes);
        ~Mesh() override;

        void RecalculateBounds();
    private:
        MeshData m_mesh_data;
        BoundingBox m_bounds;

        Vector<Rendering::SubMesh> m_sub_meshes;
    };

    class MeshFactory final {
    public:
        static Mesh *CreateQuad(float32 width, float32 height);
        static Mesh *CreatePlane(float32 width, float32 height);
        static Mesh *CreateCube(float32 size);
        static Mesh *CreateSphere(float32 radius);
    private:
        MeshFactory() = delete;
        ~MeshFactory() = delete;
    };

}