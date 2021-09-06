//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/view/render_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void RenderView::Reset() {
        m_view_cameras.Clear();
        m_view_objects.Clear();
    }

    //--------------------------------------------------------------
    RenderViewCameraData &RenderView::AddRenderCamera() {
        uint64 length = m_view_cameras.GetLength();
        m_view_cameras.Resize(length + 1);
        return m_view_cameras[length].GetData();
    }

    //--------------------------------------------------------------
    RenderViewObjectData &RenderView::AddRenderObject() {
        uint64 length = m_view_objects.GetLength();
        m_view_objects.Resize(length + 1);
        return m_view_objects[length].GetData();
    }

}