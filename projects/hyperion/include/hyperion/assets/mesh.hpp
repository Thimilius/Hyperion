#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"

//-------------------- Definition Namespace --------------------
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

    struct MeshVertexFormat {
        uint32 stride;
        Vector<Rendering::VertexAttribute> vertex_attributes;
    };

    class Mesh final : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        const MeshData &GetMeshData() const;
        const Vector<Rendering::SubMesh> &GetSubMeshes() const;
        uint32 GetSubMeshCount() const { return static_cast<uint32>(m_sub_meshes.size()); }

        inline bool IsReadAndWriteEnabled() const { return m_read_and_write_enabled; }
        inline BoundingBox GetBounds() const { return m_bounds; }

        void SetData(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes);
    public:
        static Mesh *Create(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes, bool read_and_write_enabled = false);
    protected:
        void OnDestroy() override;
    private:
        Mesh() = default;
        Mesh(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes, bool read_and_write_enabled);
    private:
        bool HasVertexAttribute(Rendering::VertexAttributeKind vertex_attribute_kind);
    private:
        static Mesh *Create();
        static BoundingBox CalculateBounds(const Vector<Vec3> &positions);
    private:
        MeshData m_mesh_data;
        MeshVertexFormat m_vertex_format;
        Vector<Rendering::SubMesh> m_sub_meshes;

        bool m_read_and_write_enabled;
        BoundingBox m_bounds;
    };

}