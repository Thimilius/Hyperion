#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"

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
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        const MeshData &GetMeshData() const;
        const Vector<Rendering::SubMesh> &GetSubMeshes() const;

        inline bool IsReadAndWriteEnabled() const { return m_read_and_write_enabled; }
        inline BoundingBox GetBounds() const { return m_bounds; }
        
        static Mesh *Create(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes, bool read_and_write_enabled = false);
    private:
        Mesh(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes, bool read_and_write_enabled);
        ~Mesh() override;

        static BoundingBox CalculateBounds(const Vector<Vec3> &positions);
    private:
        MeshData m_mesh_data;
        Vector<Rendering::SubMesh> m_sub_meshes;

        bool m_read_and_write_enabled;
        BoundingBox m_bounds;
    };

    class IMeshLoader {
    public:
        virtual ~IMeshLoader() = default;

        virtual Mesh *LoadMesh(const String &path) = 0;
    };

    class MeshFactory final {
    public:
        static Mesh *CreateQuad(float32 width, float32 height);
        static Mesh *CreatePlane(float32 width, float32 height);
        static Mesh *CreateCube(float32 size);
        static Mesh *CreateSphere(float32 radius);
        static Mesh *CreateFromFile(const String &path);
    private:
        MeshFactory() = delete;
        ~MeshFactory() = delete;
    private:
        static IMeshLoader *s_mesh_loader;
    };

}