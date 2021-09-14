#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/rendering/render_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // The data of meshes is expected to be interpreted in a specific way:
   //     - For texture coordinates the origin (0, 0) is BOTTOM LEFT.
   //       The top left corner where the origin for most images resides is therefore at (0, 1).
   //       This means that when loading in textures they probably have to be flipped.
   //     - To specify a front facing triangle, the indices have to be provided in CLOCKWISE order.

    struct MeshData {
        Array<Vector3> positions;
        Array<Vector3> normals;
        Array<Vector4> colors;
        Array<Vector2> texture0;

        Array<uint32> indices;
    };

    struct MeshVertexFormat {
        uint32 stride;
        Array<Rendering::VertexAttribute> attributes;

        inline static uint32 VERTEX_ATTRIBUTE_SIZE_POSITION = sizeof(float32) * 3;
        inline static uint32 VERTEX_ATTRIBUTE_SIZE_NORMAL = sizeof(float32) * 3;
        inline static uint32 VERTEX_ATTRIBUTE_SIZE_COLOR = sizeof(float32) * 4;
        inline static uint32 VERTEX_ATTRIBUTE_SIZE_TEXTURE0 = sizeof(float32) * 2;
    };

    class Mesh final : public Asset {
    private:
        Mesh(AssetInfo info) : Asset(info) { }
        Mesh(AssetInfo info, const MeshData &data, const Array<Rendering::SubMesh> &sub_meshes);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }

        inline const MeshData &GetData() const { return m_data; }
        inline const MeshVertexFormat &GetVertexFormat() const { return m_vertex_format; }
        inline const Array<Rendering::SubMesh> &GetSubMeshes() { return m_sub_meshes; }
        inline uint32 GetSubMeshCount() const { return static_cast<uint32>(m_sub_meshes.GetLength()); }
        void SetData(const MeshData &data, const Array<Rendering::SubMesh> &sub_meshes);

        inline BoundingBox GetBounds() const { return m_bounds; }
    private:
        static BoundingBox CalculateBounds(const Array<Vector3> &positions);
    private:
        MeshData m_data;
        MeshVertexFormat m_vertex_format;
        Array<Rendering::SubMesh> m_sub_meshes;

        BoundingBox m_bounds;
    private:
        friend class Hyperion::AssetManager;
    };

}