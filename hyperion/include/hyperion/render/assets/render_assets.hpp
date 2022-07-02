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

  struct RenderAsset {
    AssetHandle handle;
  };

  struct RenderAssetTexture2D : public RenderAsset {
    Texture2DParameters parameters;
    uint32 mipmap_count = 0;
    TexturePixelData pixels;
  };

  struct RenderAssetRenderTexture : public RenderAsset {
    RenderTextureParameters parameters;
    uint32 mipmap_count = 0;
  };

  struct RenderAssetShader : public RenderAsset {
    bool8 is_valid = false;
    ShaderData data;
  };

  struct RenderAssetMaterial : public RenderAsset {
    AssetHandle shader_handle;
    MaterialPropertyCollection properties;
  };

  struct RenderAssetMesh : public RenderAsset {
    MeshData data;
    MeshVertexFormat vertex_format;
    MeshIndexFormat index_format = MeshIndexFormat::UInt32;
    MeshUpdateType update_type = MeshUpdateType::Static;
    SubMeshes sub_meshes;
  };

}
