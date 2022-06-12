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
    inline Array<AssetHandle> &GetTexture2DAssetsToUnload() { return m_texture_2ds_to_unload; }
    inline Array<AssetHandle> &GetRenderTextureAssetsToUnload() { return m_render_textures_to_unload; }
    inline Array<AssetHandle> &GetShaderAssetsToUnload() { return m_shaders_to_unload; }
    inline Array<AssetHandle> &GetMaterialAssetsToUnload() { return m_materials_to_unload; }
    inline Array<AssetHandle> &GetMeshAssetsToUnload() { return m_meshes_to_unload; }

    RenderAssetTexture2D &AddTexture2DAssetToLoad();
    RenderAssetRenderTexture &AddRenderTextureAssetToLoad();
    RenderAssetShader &AddShaderAssetToLoad();
    RenderAssetMaterial &AddMaterialAssetToLoad();
    RenderAssetMesh &AddMeshAssetToLoad();
    void AddTexture2DToUnload(AssetHandle asset_handle);
    void AddRenderTextureToUnload(AssetHandle asset_handle);
    void AddShaderToUnload(AssetHandle asset_handle);
    void AddMaterialToUnload(AssetHandle asset_handle);
    void AddMeshToUnload(AssetHandle asset_handle);
  private:
    Array<RenderAssetTexture2D> m_texture_2ds_to_load;
    Array<RenderAssetRenderTexture> m_render_textures_to_load;
    Array<RenderAssetShader> m_shaders_to_load;
    Array<RenderAssetMaterial> m_materials_to_load;
    Array<RenderAssetMesh> m_meshes_to_load;
    Array<AssetHandle> m_texture_2ds_to_unload;
    Array<AssetHandle> m_render_textures_to_unload;
    Array<AssetHandle> m_shaders_to_unload;
    Array<AssetHandle> m_materials_to_unload;
    Array<AssetHandle> m_meshes_to_unload;
  };

}
