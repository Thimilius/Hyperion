//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        render_frame->SetCamera(0);
        render_frame->Clear(ClearFlags::All, Color::Cyan());
        render_frame->DrawAll();
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

}