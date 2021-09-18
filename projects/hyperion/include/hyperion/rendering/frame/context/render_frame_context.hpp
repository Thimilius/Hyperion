#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/context/render_frame_context_assets.hpp"
#include "hyperion/rendering/frame/context/render_frame_context_camera.hpp"
#include "hyperion/rendering/frame/context/render_frame_context_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrameContext final : public INonCopyable {
    public:
        void Clear();

        Array<RenderFrameContextAssetMaterial> &GetMaterialsToLoad() { return m_materials_to_load; }
        Array<RenderFrameContextAssetMesh> &GetMeshesToLoad() { return m_meshes_to_load; }
        Array<RenderFrameContextAssetShader> &GetShadersToLoad() { return m_shaders_to_load; }
        Array<AssetId> &GetAssetsToUnload() { return m_assets_to_unload; }

        const Array<RenderFrameContextCamera> &GetCameras() const { return m_cameras; }
        const Array<RenderFrameContextObjectMesh> &GetMeshObjects() const { return m_mesh_objects; }
        const Array<RenderFrameContextObjectSprite> &GetSpriteObjects() const { return m_sprite_objects; }

        RenderFrameContextAssetMaterial &AddMaterialAssetToLoad();
        RenderFrameContextAssetMesh &AddMeshAssetToLoad();
        RenderFrameContextAssetShader &AddShaderAssetToLoad();
        void AddAssetToUnload(AssetId asset_id);

        RenderFrameContextCamera &AddCamera();
        RenderFrameContextObjectMesh &AddMeshObject();
        RenderFrameContextObjectSprite &AddSpriteObject();
    private:
        Array<RenderFrameContextAssetMaterial> m_materials_to_load;
        Array<RenderFrameContextAssetMesh> m_meshes_to_load;
        Array<RenderFrameContextAssetShader> m_shaders_to_load;
        Array<AssetId> m_assets_to_unload;

        Array<RenderFrameContextCamera> m_cameras;
        Array<RenderFrameContextObjectMesh> m_mesh_objects;
        Array<RenderFrameContextObjectSprite> m_sprite_objects;
    };

}