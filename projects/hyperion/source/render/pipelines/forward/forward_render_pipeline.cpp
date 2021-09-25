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
        // HACK: We need a better way to get/set a camera.
        const RenderFrameContextCamera &camera = render_frame->GetContext().GetCameras()[0];
        CullingParameters culling_parameters;
        culling_parameters.matrix = camera.view_projection_matrix;
        culling_parameters.mask = camera.culling_mask;
        CullingResults culling_results = render_frame->Cull(culling_parameters);

        render_frame->SetCamera(0);

        RenderFrameCommandBuffer command_buffer;
        command_buffer.ClearRenderTarget(ClearFlags::All, Color::Cyan());
        render_frame->ExecuteCommandBuffer(command_buffer);

        DrawingParametes drawing_parameters;
        drawing_parameters.filter_mask = LayerMask::Everything;
        drawing_parameters.per_object_data = PerObjectData::LightIndices;
        render_frame->DrawMeshes(culling_results, drawing_parameters);

        render_frame->DrawGizmos();
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

}