//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

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
        SetupLighting(render_frame->GetContext(), command_buffer);
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

    //--------------------------------------------------------------
    void ForwardRenderPipeline::SetupLighting(RenderFrameContext &context, RenderFrameCommandBuffer &command_buffer) {
        RenderFrameContextEnvironment environment = context.GetEnvironment();
        auto &lights = context.GetLights();
        RenderBuffer render_buffer_lighting = RenderBuffer(sizeof(LightingBuffer));
        LightingBuffer *lighting_buffer = reinterpret_cast<LightingBuffer *>(render_buffer_lighting.GetData().GetData());

        lighting_buffer->ambient_color = environment.ambient_light.intensity * environment.ambient_light.color;

        // Because of how the frame context gets populated, we know that the first light (if present and directional) is the main light.
        auto light_it = lights.begin();
        if (light_it != lights.end() && light_it->type == LightType::Directional) {
            const RenderFrameContextLight &main_light = *light_it;
            CopyFrameLightToLight(main_light, lighting_buffer->main_light);
        }

        uint64 point_light_index = 0;
        for (const RenderFrameContextLight &light : lights) {
            if (light.type == LightType::Point) {
                // HACK: For now we just ignore every light that is beyond our hardcoded limit.

                if (point_light_index < 128) {
                    CopyFrameLightToLight(light, lighting_buffer->point_lights[point_light_index++]);
                }
            }
        }

        command_buffer.SetGlobalBuffer(0, std::move(render_buffer_lighting));
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::CopyFrameLightToLight(const RenderFrameContextLight &frame_light, Light &light) {
        light.color = frame_light.color;
        light.intensity = frame_light.intensity;
        light.direction = frame_light.direction;
        light.position = frame_light.position;
        light.range = frame_light.range;
        light.spot_inner_radius = frame_light.inner_spot_radius;
        light.spot_outer_radius = frame_light.outer_spot_radius;
    }

}