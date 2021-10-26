//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    RenderTexture *CreateRenderTexture(uint32 width, uint32 height) {
        TextureAttributes render_texture_attributes;
        render_texture_attributes.filter = TextureFilter::Point;
        render_texture_attributes.use_mipmaps = false;

        RenderTextureParameters render_texture_parameters;
        render_texture_parameters.width = width;
        render_texture_parameters.height = height;
        render_texture_parameters.attachments = {
            { RenderTextureFormat::Depth24Stencil8, render_texture_attributes },
            { RenderTextureFormat::RGBA32, render_texture_attributes }
        };

        return AssetManager::CreateRenderTexture(render_texture_parameters);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize() {
        m_target_render_texture = CreateRenderTexture(m_render_target_width, m_render_target_height);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        if (m_target_render_texture->GetWidth() != m_render_target_width || m_target_render_texture->GetHeight() != m_render_target_height) {
            m_target_render_texture->Resize(m_render_target_width, m_render_target_height);
        }

        // HACK: We need a better way to get/set a camera.
        const RenderFrameContextCamera &camera = render_frame->GetContext().GetCameras()[0];
        CullingParameters culling_parameters;
        culling_parameters.matrix = camera.view_projection_matrix;
        culling_parameters.mask = camera.culling_mask;
        CullingResults culling_results = render_frame->Cull(culling_parameters);

        render_frame->SetCamera(0);

        {
            RenderFrameCommandBuffer command_buffer;
            command_buffer.SetRenderTarget(RenderTargetId::Default());
            command_buffer.ClearRenderTarget(ClearFlags::All, Color::Black());
            command_buffer.SetRenderTarget(m_target_render_texture->GetRenderTargetId());
            command_buffer.ClearRenderTarget(ClearFlags::All, camera.background_color);
            ForwardRenderLighting::SetupLighting(render_frame->GetContext(), command_buffer);
            render_frame->ExecuteCommandBuffer(command_buffer);
        }

        DrawingParametes drawing_parameters_opaque;
        drawing_parameters_opaque.filter_mask = LayerMask::Everything;
        drawing_parameters_opaque.per_object_data = PerObjectData::LightIndices;
        drawing_parameters_opaque.render_order = ShaderRenderOrder::Opaque;
        drawing_parameters_opaque.sorting_settings.camera_position = camera.position;
        drawing_parameters_opaque.sorting_settings.criteria = SortingCriteria::Opaque;
        render_frame->DrawMeshes(culling_results, drawing_parameters_opaque);

        DrawingParametes drawing_parameters_transparent;
        drawing_parameters_transparent.filter_mask = LayerMask::Everything;
        drawing_parameters_transparent.per_object_data = PerObjectData::Nothing;
        drawing_parameters_transparent.render_order = ShaderRenderOrder::Transparent;
        drawing_parameters_transparent.sorting_settings.camera_position = camera.position;
        drawing_parameters_transparent.sorting_settings.criteria = SortingCriteria::Transparent;
        render_frame->DrawMeshes(culling_results, drawing_parameters_transparent);

        render_frame->DrawEditorGizmos();
        render_frame->DrawUI();

        {
            RenderFrameCommandBuffer command_buffer;
            if (m_should_blit_to_screen) {
                command_buffer.Blit(RenderTargetId::Default(), m_target_render_texture->GetRenderTargetId());
            } else {
                command_buffer.SetRenderTarget(RenderTargetId::Default());
            }
            render_frame->ExecuteCommandBuffer(command_buffer);
        }

        render_frame->DrawEditorUI();
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::SetRenderTargetSize(uint32 width, uint32 height) {
        m_render_target_width = width;
        m_render_target_height = height;
    }

}