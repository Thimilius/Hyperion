#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/render/types/render_types_mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // The data of meshes is expected to be interpreted in a specific way:
    //     - For texture coordinates the origin (0, 0) is BOTTOM LEFT.
    //       The top left corner where the origin for most images resides is therefore at (0, 1).
    //       This means that when loading in textures they probably have to be flipped.
    //     - To specify a front facing triangle, the indices have to be provided in CLOCKWISE order.

    class Mesh final : public Asset {
    private:
        Mesh(AssetInfo info) : Asset(info) { }
        Mesh(AssetInfo info, const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        inline const Rendering::MeshVertexFormat &GetVertexFormat() const { return m_vertex_format; }
        inline const Rendering::SubMeshes &GetSubMeshes() { return m_sub_meshes; }
        inline uint32 GetSubMeshCount() const { return static_cast<uint32>(m_sub_meshes.GetLength()); }

        const Rendering::MeshData &GetData() const;
        void SetData(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes);

        inline BoundingBox GetBounds() const { return m_bounds; }
    private:
        void SetDataInternal(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes);
    private:
        Rendering::MeshData m_data;
        Rendering::MeshVertexFormat m_vertex_format;
        Rendering::SubMeshes m_sub_meshes;

        BoundingBox m_bounds;
    private:
        friend class Hyperion::AssetManager;
        friend class Hyperion::AssetLoadSystem;
    };

}