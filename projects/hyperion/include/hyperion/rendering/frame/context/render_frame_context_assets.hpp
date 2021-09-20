#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/rendering/types/render_types_material.hpp"
#include "hyperion/rendering/types/render_types_mesh.hpp"
#include "hyperion/rendering/types/render_types_shader.hpp"
#include "hyperion/rendering/types/render_types_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextAsset {
        AssetId id;
    };

    struct RenderFrameContextAssetTexture2D : public RenderFrameContextAsset {
        Texture2DParameters parameters;
        uint32 mipmap_count;
        TexturePixelData pixels;
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