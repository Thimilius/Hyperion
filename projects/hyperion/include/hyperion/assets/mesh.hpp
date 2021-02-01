#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"

namespace Hyperion {

    // The data of meshes is expected to be interpreted in a specific way:
    //     - For texture coordinates the origin (0, 0) is BOTTOM LEFT.
    //       The top left corner where the origin for most images resides is therefore at (0, 1).
    //       This means that when loading in textures they probably have to be flipped.
    //     - To specify a front facing triangle, the indices have to be provided in CLOCKWISE order.

    struct MeshData {
        Vector<Vec3> positions;
        Vector<Vec3> normals;
        Vector<Vec4> colors;
        Vector<Vec2> texture0;

        Vector<uint32> indices;
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
    protected:
        void OnDestroy() override;
    private:
        Mesh() = default;
        Mesh(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes, bool read_and_write_enabled);

        static Mesh *Create();
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

        static void Initialize();
        static void Shutdown();
    private:
        inline static IMeshLoader *s_mesh_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}