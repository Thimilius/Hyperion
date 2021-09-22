//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        render_frame->SetCamera(0);

        RenderFrameCommandBuffer command_buffer;
        command_buffer.ClearRenderTarget(ClearFlags::All, Color::Cyan());
        render_frame->ExecuteCommandBuffer(command_buffer);

        render_frame->DrawMeshes();
        render_frame->DrawGizmos();
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

}