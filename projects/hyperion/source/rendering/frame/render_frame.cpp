//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void RenderFrame::Reset() {
        m_frame_assets_to_unload.Clear();

        m_frame_cameras.Clear();
        m_frame_objects.Clear();
    }

    //--------------------------------------------------------------
    void RenderFrame::AddFrameAssetToUnload(AssetId asset_id) {
        HYP_ASSERT(!m_frame_assets_to_unload.Contains(asset_id));
        m_frame_assets_to_unload.Add(asset_id);
    }

    //--------------------------------------------------------------
    RenderFrameCameraData &RenderFrame::AddFrameCamera() {
        uint64 length = m_frame_cameras.GetLength();
        m_frame_cameras.Resize(length + 1);
        return m_frame_cameras[length].GetData();
    }

    //--------------------------------------------------------------
    RenderFrameObjectData &RenderFrame::AddFrameObject() {
        uint64 length = m_frame_objects.GetLength();
        m_frame_objects.Resize(length + 1);
        return m_frame_objects[length].GetData();
    }

}