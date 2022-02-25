//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderFrameContext::Clear() {
    ClearAssets();

    m_cameras.Clear();
    m_lights.Clear();
    m_mesh_objects.Clear();
    m_sprite_objects.Clear();
    m_ui_objects.Clear();
    m_editor_ui_objects.Clear();
  }

  //--------------------------------------------------------------
  void RenderFrameContext::ClearAssets() {
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
  RenderFrameContextAssetTexture2D &RenderFrameContext::AddTexture2DAssetToLoad() {
    m_texture_2ds_to_load.Resize(m_texture_2ds_to_load.GetLength() + 1);
    return m_texture_2ds_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextAssetRenderTexture &RenderFrameContext::AddRenderTextureAssetToLoad() {
    m_render_textures_to_load.Resize(m_render_textures_to_load.GetLength() + 1);
    return m_render_textures_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextAssetMaterial &RenderFrameContext::AddMaterialAssetToLoad() {
    m_materials_to_load.Resize(m_materials_to_load.GetLength() + 1);
    return m_materials_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextAssetMesh &RenderFrameContext::AddMeshAssetToLoad() {
    m_meshes_to_load.Resize(m_meshes_to_load.GetLength() + 1);
    return m_meshes_to_load.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextAssetShader &RenderFrameContext::AddShaderAssetToLoad() {
    m_shaders_to_load.Resize(m_shaders_to_load.GetLength() + 1);
    return m_shaders_to_load.GetLast();
  }

  //--------------------------------------------------------------
  void RenderFrameContext::AddTexture2DToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_texture_2ds_to_unload.Contains(asset_id));
    m_texture_2ds_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderFrameContext::AddRenderTextureToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_render_textures_to_unload.Contains(asset_id));
    m_render_textures_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderFrameContext::AddShaderToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_shaders_to_unload.Contains(asset_id));
    m_shaders_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderFrameContext::AddMaterialToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_materials_to_unload.Contains(asset_id));
    m_materials_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  void RenderFrameContext::AddMeshToUnload(AssetId asset_id) {
    HYP_ASSERT(!m_meshes_to_unload.Contains(asset_id));
    m_meshes_to_unload.Add(asset_id);
  }

  //--------------------------------------------------------------
  RenderFrameContextCamera &RenderFrameContext::AddCamera() {
    m_cameras.Resize(m_cameras.GetLength() + 1);
    return m_cameras.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextLight &RenderFrameContext::AddLight() {
    m_lights.Resize(m_lights.GetLength() + 1);
    return m_lights.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectMesh &RenderFrameContext::AddMeshObject() {
    m_mesh_objects.Resize(m_mesh_objects.GetLength() + 1);
    return m_mesh_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectSprite &RenderFrameContext::AddSpriteObject() {
    m_sprite_objects.Resize(m_sprite_objects.GetLength() + 1);
    return m_sprite_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectUI &RenderFrameContext::AddUIObject() {
    m_ui_objects.Resize(m_ui_objects.GetLength() + 1);
    return m_ui_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectUI &RenderFrameContext::AddEditorUIObject() {
    m_editor_ui_objects.Resize(m_editor_ui_objects.GetLength() + 1);
    return m_editor_ui_objects.GetLast();
  }

}