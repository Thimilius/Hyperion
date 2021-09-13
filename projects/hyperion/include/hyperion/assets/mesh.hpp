#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetManager;
}

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

    class Mesh final : public Asset {
    private:
        Mesh(AssetInfo info) : Asset(info) { }
        Mesh(AssetInfo info, const MeshData &data);
        ~Mesh() = default;
    public:
        inline AssetType GetAssetType() const override { return AssetType::Mesh; }
    private:
        static BoundingBox CalculateBounds(const Array<Vector3> &positions);
    private:
        MeshData m_data;
        BoundingBox m_bounds;
    private:
        friend class Hyperion::AssetManager;
    };

}