#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/context/render_frame_context_camera.hpp"
#include "hyperion/rendering/frame/context/render_frame_context_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrameContext final : public INonCopyable {
    public:
        void Clear();

        const Array<Asset *> &GetAssetsToLoad() const { return m_assets_to_load; }
        const Array<AssetId> &GetAssetsToUnload() const { return m_assets_to_unload; }

        const Array<RenderFrameContextCamera> &GetCameras() const { return m_cameras; }
        const Array<RenderFrameContextMeshObject> &GetMeshObjects() const { return m_mesh_objects; }
        const Array<RenderFrameContextSpriteObject> &GetSpriteObjects() const { return m_sprite_objects; }

        void AddAssetToLoad(Asset *asset);
        void AddAssetToUnload(AssetId asset_id);

        RenderFrameContextCamera &AddCamera();
        RenderFrameContextMeshObject &AddMeshObject();
        RenderFrameContextSpriteObject &AddSpriteObject();
    private:
        Array<AssetId> m_assets_to_unload;
        Array<Asset *> m_assets_to_load;

        Array<RenderFrameContextCamera> m_cameras;
        Array<RenderFrameContextMeshObject> m_mesh_objects;
        Array<RenderFrameContextSpriteObject> m_sprite_objects;
    };

}