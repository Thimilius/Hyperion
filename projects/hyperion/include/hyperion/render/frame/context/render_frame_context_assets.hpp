//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/render/types/render_types_material.hpp"
#include "hyperion/render/types/render_types_mesh.hpp"
#include "hyperion/render/types/render_types_shader.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

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

    struct RenderFrameContextAssetRenderTexture : public RenderFrameContextAsset {
        RenderTextureParameters parameters;
        uint32 mipmap_count;
    };

    struct RenderFrameContextAssetShader : public RenderFrameContextAsset {
        bool8 is_valid;
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