#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame_camera.hpp"
#include "hyperion/rendering/frame/render_frame_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Reset();

        const Array<RenderFrameCamera> &GetFrameCameras() const { return m_frame_cameras; }
        const Array<RenderFrameObject> &GetFrameObjects() const { return m_frame_objects; }
        const Array<AssetId> &GetFrameAssetsToUnload() const { return m_frame_assets_to_unload; }

        RenderFrameCameraData &AddFrameCamera();
        RenderFrameObjectData &AddFrameObject();
        void AddFrameAssetToUnload(AssetId asset_id);
    private:
        Array<RenderFrameCamera> m_frame_cameras;
        Array<RenderFrameObject> m_frame_objects;
        Array<AssetId> m_frame_assets_to_unload;
    };

}