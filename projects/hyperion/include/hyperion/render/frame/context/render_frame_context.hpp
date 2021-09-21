#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/context/render_frame_context_assets.hpp"
#include "hyperion/render/frame/context/render_frame_context_camera.hpp"
#include "hyperion/render/frame/context/render_frame_context_light.hpp"
#include "hyperion/render/frame/context/render_frame_context_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrameContext final : public INonCopyable {
    public:
        void Clear();

        Array<AssetId> &GetAssetsToUnload() { return m_assets_to_unload; }
        Array<RenderFrameContextAssetTexture2D> &GetTexture2DAssetsToLoad() {return m_texture_2ds_to_load; }
        Array<RenderFrameContextAssetShader> &GetShaderAssetsToLoad() { return m_shaders_to_load; }
        Array<RenderFrameContextAssetMaterial> &GetMaterialAssetsToLoad() { return m_materials_to_load; }
        Array<RenderFrameContextAssetMesh> &GetMeshAssetsToLoad() { return m_meshes_to_load; }

        const Array<RenderFrameContextCamera> &GetCameras() const { return m_cameras; }
        const Array<RenderFrameContextLight> &GetLights() const { return m_lights; }
        const Array<RenderFrameContextObjectMesh> &GetMeshObjects() const { return m_mesh_objects; }
        const Array<RenderFrameContextObjectSprite> &GetSpriteObjects() const { return m_sprite_objects; }

        void AddAssetToUnload(AssetId asset_id);
        RenderFrameContextAssetTexture2D &AddTexture2DAssetToLoad();
        RenderFrameContextAssetShader &AddShaderAssetToLoad();
        RenderFrameContextAssetMaterial &AddMaterialAssetToLoad();
        RenderFrameContextAssetMesh &AddMeshAssetToLoad();

        RenderFrameContextCamera &AddCamera();
        RenderFrameContextLight &AddLight();
        RenderFrameContextObjectMesh &AddMeshObject();
        RenderFrameContextObjectSprite &AddSpriteObject();
    private:
        Array<AssetId> m_assets_to_unload;
        Array<RenderFrameContextAssetTexture2D> m_texture_2ds_to_load;
        Array<RenderFrameContextAssetShader> m_shaders_to_load;
        Array<RenderFrameContextAssetMaterial> m_materials_to_load;
        Array<RenderFrameContextAssetMesh> m_meshes_to_load;

        Array<RenderFrameContextCamera> m_cameras;
        Array<RenderFrameContextLight> m_lights;
        Array<RenderFrameContextObjectMesh> m_mesh_objects;
        Array<RenderFrameContextObjectSprite> m_sprite_objects;
    };

}