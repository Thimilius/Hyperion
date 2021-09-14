#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame_camera.hpp"
#include "hyperion/rendering/frame/render_frame_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Clear();

        const Array<Asset *> &GetAssetsToLoad() const { return m_assets_to_load; }
        const Array<AssetId> &GetAssetsToUnload() const { return m_assets_to_unload; }

        const Array<RenderFrameCamera> &GetCameras() const { return m_cameras; }
        const Array<RenderFrameMeshObject> &GetMeshObjects() const { return m_mesh_objects; }
        const Array<RenderFrameSpriteObject> &GetSpriteObjects() const { return m_sprite_objects; }

        void AddAssetToLoad(Asset *asset);
        void AddAssetToUnload(AssetId asset_id);

        RenderFrameCamera &AddCamera();
        RenderFrameMeshObject &AddMeshObject();
        RenderFrameSpriteObject &AddSpriteObject();
    private:
        Array<AssetId> m_assets_to_unload;
        Array<Asset *> m_assets_to_load;

        Array<RenderFrameCamera> m_cameras;
        Array<RenderFrameMeshObject> m_mesh_objects;
        Array<RenderFrameSpriteObject> m_sprite_objects;
    };

}