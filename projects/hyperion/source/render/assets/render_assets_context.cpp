//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/assets/render_assets_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderAssetContext::Clear() {
    m_texture_2ds_to_load.Clear();
    m_render_textures_to_load.Clear();
    m_shaders_to_load.Clear();
    m_materials_to_load.Clear();
    m_meshes_to_load.Clear();

    m_texture_2ds_to_unload.Clear();
    m_render_textures_to_unload.Clear();
    m_shaders_to_unload.Clear();
    m_materials_to_unload.Clear();
    m_meshes_to_unload.Clear();
  }

  //--------------------------------------------------------------
  RenderAssetTexture2D &RenderAssetContext::AddTexture2DAssetToLoad() {
    m_texture_2ds_to_load.Resize(m_texture_2ds_to_load.GetLength() + 1);
    return m_texture_2ds_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderAssetRenderTexture &RenderAssetContext::AddRenderTextureAssetToLoad() {
    m_render_textures_to_load.Resize(m_render_textures_to_load.GetLength() + 1);
    return m_render_textures_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderAssetMaterial &RenderAssetContext::AddMaterialAssetToLoad() {
    m_materials_to_load.Resize(m_materials_to_load.GetLength() + 1);
    return m_materials_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderAssetMesh &RenderAssetContext::AddMeshAssetToLoad() {
    m_meshes_to_load.Resize(m_meshes_to_load.GetLength() + 1);
    return m_meshes_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderAssetShader &RenderAssetContext::AddShaderAssetToLoad() {
    m_shaders_to_load.Resize(m_shaders_to_load.GetLength() + 1);
    return m_shaders_to_load.GetLast();
  }

  //--------------------------------------------------------------
  void RenderAssetContext::AddTexture2DToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_texture_2ds_to_unload.Contains(asset_id));
    m_texture_2ds_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderAssetContext::AddRenderTextureToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_render_textures_to_unload.Contains(asset_id));
    m_render_textures_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderAssetContext::AddShaderToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_shaders_to_unload.Contains(asset_id));
    m_shaders_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderAssetContext::AddMaterialToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_materials_to_unload.Contains(asset_id));
    m_materials_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderAssetContext::AddMeshToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_meshes_to_unload.Contains(asset_id));
    m_meshes_to_unload.Add(asset_id);
  }

}