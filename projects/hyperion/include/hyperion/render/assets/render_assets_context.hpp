//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/assets/render_assets.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderAssetContext final : public INonCopyable {
  public:
    void Clear();

    inline Array<RenderAssetTexture2D> &GetTexture2DAssetsToLoad() { return m_texture_2ds_to_load; }
    inline Array<RenderAssetRenderTexture> &GetRenderTextureAssetsToLoad() { return m_render_textures_to_load; }
    inline Array<RenderAssetShader> &GetShaderAssetsToLoad() { return m_shaders_to_load; }
    inline Array<RenderAssetMaterial> &GetMaterialAssetsToLoad() { return m_materials_to_load; }
    inline Array<RenderAssetMesh> &GetMeshAssetsToLoad() { return m_meshes_to_load; }
    inline Array<AssetId> &GetTexture2DAssetsToUnload() { return m_texture_2ds_to_unload; }
    inline Array<AssetId> &GetRenderTextureAssetsToUnload() { return m_render_textures_to_unload; }
    inline Array<AssetId> &GetShaderAssetsToUnload() { return m_shaders_to_unload; }
    inline Array<AssetId> &GetMaterialAssetsToUnload() { return m_materials_to_unload; }
    inline Array<AssetId> &GetMeshAssetsToUnload() { return m_meshes_to_unload; }

    RenderAssetTexture2D &AddTexture2DAssetToLoad();
    RenderAssetRenderTexture &AddRenderTextureAssetToLoad();
    RenderAssetShader &AddShaderAssetToLoad();
    RenderAssetMaterial &AddMaterialAssetToLoad();
    RenderAssetMesh &AddMeshAssetToLoad();
    void AddTexture2DToUnload(AssetId asset_id);
    void AddRenderTextureToUnload(AssetId asset_id);
    void AddShaderToUnload(AssetId asset_id);
    void AddMaterialToUnload(AssetId asset_id);
    void AddMeshToUnload(AssetId asset_id);
  private:
    Array<RenderAssetTexture2D> m_texture_2ds_to_load;
    Array<RenderAssetRenderTexture> m_render_textures_to_load;
    Array<RenderAssetShader> m_shaders_to_load;
    Array<RenderAssetMaterial> m_materials_to_load;
    Array<RenderAssetMesh> m_meshes_to_load;
    Array<AssetId> m_texture_2ds_to_unload;
    Array<AssetId> m_render_textures_to_unload;
    Array<AssetId> m_shaders_to_unload;
    Array<AssetId> m_materials_to_unload;
    Array<AssetId> m_meshes_to_unload;
  };

}
