#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/rendering/types/render_types_material.hpp"
#include "hyperion/rendering/types/render_types_mesh.hpp"
#include "hyperion/rendering/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextAsset {
        AssetId id;
    };

    struct RenderFrameContextAssetShader : public RenderFrameContextAsset {
        ShaderData data;
    };

    struct RenderFrameContextAssetMaterial : public RenderFrameContextAsset {
        AssetId shader_id;
        MaterialPropertyCollection properties;
    };

    struct RenderFrameContextAssetMesh : public RenderFrameContextAsset {
        MeshData data;
        MeshVertexFormat vertex_format;
        SubMeshes sub_meshes;
    };

}