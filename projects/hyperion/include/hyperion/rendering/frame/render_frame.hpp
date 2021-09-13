#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame_camera.hpp"
#include "hyperion/rendering/frame/render_frame_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Reset();

        const Array<RenderFrameCamera> &GetFrameCameras() const { return m_frame_cameras; }
        const Array<RenderFrameMeshObject> &GetFrameMeshObjects() const { return m_frame_mesh_objects; }
        const Array<RenderFrameSpriteObject> &GetFrameSpriteObjects() const { return m_frame_sprite_objects; }
        const Array<AssetId> &GetFrameAssetsToUnload() const { return m_frame_assets_to_unload; }

        RenderFrameCamera &AddFrameCamera();
        RenderFrameMeshObject &AddFrameMeshObject();
        RenderFrameSpriteObject &AddFrameSpriteObject();
        void AddFrameAssetToUnload(AssetId asset_id);
    private:
        Array<RenderFrameCamera> m_frame_cameras;
        Array<RenderFrameMeshObject> m_frame_mesh_objects;
        Array<RenderFrameSpriteObject> m_frame_sprite_objects;
        Array<AssetId> m_frame_assets_to_unload;
    };

}