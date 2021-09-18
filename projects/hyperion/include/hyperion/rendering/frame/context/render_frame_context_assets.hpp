#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/rendering/types/render_types_mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextAsset {
        AssetId asset_id;
    };

    struct RenderFrameContextAssetShader : public RenderFrameContextAsset {

    };

    struct RenderFrameContextAssetMaterial : public RenderFrameContextAsset {

    };

    struct RenderFrameContextAssetMesh : public RenderFrameContextAsset {
        MeshData data;
        MeshVertexFormat vertex_format;
        SubMeshes sub_meshes;
    };

}