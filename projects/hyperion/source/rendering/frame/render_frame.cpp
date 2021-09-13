//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void RenderFrame::Reset() {
        m_frame_cameras.Clear();
        m_frame_objects.Clear();
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