//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderFrame::Clear() {
        m_context.Clear();
    }

}