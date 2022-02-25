//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/context/render_frame_context_assets.hpp"
#include "hyperion/render/frame/context/render_frame_context_camera.hpp"
#include "hyperion/render/frame/context/render_frame_context_environment.hpp"
#include "hyperion/render/frame/context/render_frame_context_light.hpp"
#include "hyperion/render/frame/context/render_frame_context_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderFrameContext final : public INonCopyable {
  public:
    void Clear();
    void ClearAssets();

    inline Array<RenderFrameContextAssetTexture2D> &GetTexture2DAssetsToLoad() { return m_texture_2ds_to_load; }
    inline Array<RenderFrameContextAssetRenderTexture> &GetRenderTextureAssetsToLoad() { return m_render_textures_to_load; }
    inline Array<RenderFrameContextAssetShader> &GetShaderAssetsToLoad() { return m_shaders_to_load; }
    inline Array<RenderFrameContextAssetMaterial> &GetMaterialAssetsToLoad() { return m_materials_to_load; }
    inline Array<RenderFrameContextAssetMesh> &GetMeshAssetsToLoad() { return m_meshes_to_load; }
    inline Array<AssetId> &GetTexture2DAssetsToUnload() { return m_texture_2ds_to_unload; }
    inline Array<AssetId> &GetRenderTextureAssetsToUnload() { return m_render_textures_to_unload; }
    inline Array<AssetId> &GetShaderAssetsToUnload() { return m_shaders_to_unload; }
    inline Array<AssetId> &GetMaterialAssetsToUnload() { return m_materials_to_unload; }
    inline Array<AssetId> &GetMeshAssetsToUnload() { return m_meshes_to_unload; }

    inline const RenderFrameContextEnvironment &GetEnvironment() const { return m_environment; }
    inline const Array<RenderFrameContextCamera> &GetCameras() const { return m_cameras; }
    inline const Array<RenderFrameContextLight> &GetLights() const { return m_lights; }
    inline const Array<RenderFrameContextObjectMesh> &GetMeshObjects() const { return m_mesh_objects; }
    inline const Array<RenderFrameContextObjectSprite> &GetSpriteObjects() const { return m_sprite_objects; }
    inline const Array<RenderFrameContextObjectUI> &GetUIObjects() const { return m_ui_objects; }
    inline const Array<RenderFrameContextObjectUI> &GetEditorUIObjects() const { return m_editor_ui_objects; }

    RenderFrameContextAssetTexture2D &AddTexture2DAssetToLoad();
    RenderFrameContextAssetRenderTexture &AddRenderTextureAssetToLoad();
    RenderFrameContextAssetShader &AddShaderAssetToLoad();
    RenderFrameContextAssetMaterial &AddMaterialAssetToLoad();
    RenderFrameContextAssetMesh &AddMeshAssetToLoad();
    void AddTexture2DToUnload(AssetId asset_id);
    void AddRenderTextureToUnload(AssetId asset_id);
    void AddShaderToUnload(AssetId asset_id);
    void AddMaterialToUnload(AssetId asset_id);
    void AddMeshToUnload(AssetId asset_id);

    inline RenderFrameContextEnvironment &GetEnvironment() { return m_environment; }
    RenderFrameContextCamera &AddCamera();
    RenderFrameContextLight &AddLight();
    RenderFrameContextObjectMesh &AddMeshObject();
    RenderFrameContextObjectSprite &AddSpriteObject();
    RenderFrameContextObjectUI &AddUIObject();
    RenderFrameContextObjectUI &AddEditorUIObject();
  private:
    Array<RenderFrameContextAssetTexture2D> m_texture_2ds_to_load;
    Array<RenderFrameContextAssetRenderTexture> m_render_textures_to_load;
    Array<RenderFrameContextAssetShader> m_shaders_to_load;
    Array<RenderFrameContextAssetMaterial> m_materials_to_load;
    Array<RenderFrameContextAssetMesh> m_meshes_to_load;
    Array<AssetId> m_texture_2ds_to_unload;
    Array<AssetId> m_render_textures_to_unload;
    Array<AssetId> m_shaders_to_unload;
    Array<AssetId> m_materials_to_unload;
    Array<AssetId> m_meshes_to_unload;

    RenderFrameContextEnvironment m_environment;
    Array<RenderFrameContextCamera> m_cameras;
    Array<RenderFrameContextLight> m_lights;
    Array<RenderFrameContextObjectMesh> m_mesh_objects;
    Array<RenderFrameContextObjectSprite> m_sprite_objects;
    Array<RenderFrameContextObjectUI> m_ui_objects;
    Array<RenderFrameContextObjectUI> m_editor_ui_objects;
  };

}