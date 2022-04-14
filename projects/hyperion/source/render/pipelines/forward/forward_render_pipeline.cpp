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
  void ForwardRenderPipeline::Initialize() {
    TextureAttributes render_texture_attributes;
    render_texture_attributes.filter = TextureFilter::Point;
    render_texture_attributes.use_mipmaps = false;

    if (m_should_resize_to_screen) {
      m_render_target_width = Display::GetWidth();
      m_render_target_height = Display::GetHeight();
    }

    RenderTextureParameters render_texture_parameters;
    render_texture_parameters.width = m_render_target_width;
    render_texture_parameters.height = m_render_target_height;
    render_texture_parameters.attachments = {
      { RenderTextureFormat::RGBA32, render_texture_attributes, true },
      { RenderTextureFormat::Depth24Stencil8, render_texture_attributes, false },
    };

    m_target_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::Render(RenderFrame *render_frame, const Array<const RenderFrameContextCamera *> cameras) {
    if (m_should_resize_to_screen) {
      if (Display::HasChangedSize()) {
        m_render_target_width = Display::GetWidth();
        m_render_target_height = Display::GetHeight();
        m_target_render_texture->Resize(m_render_target_width, m_render_target_height);
      }
    } else {
      if (m_target_render_texture->GetWidth() != m_render_target_width || m_target_render_texture->GetHeight() != m_render_target_height) {
        m_target_render_texture->Resize(m_render_target_width, m_render_target_height);
      }
    }

    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(RenderTargetId::Default());
      command_buffer.ClearRenderTarget(ClearFlags::All, Color::Black());
      command_buffer.SetRenderTarget(m_target_render_texture->GetRenderTargetId());
      ForwardRenderLighting::SetupLighting(render_frame->GetContext(), command_buffer);
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    for (const RenderFrameContextCamera *camera : cameras) {
      RenderCamera(render_frame, camera);
    }

    render_frame->DrawUI();
    {
      RenderCommandBuffer command_buffer;
      if (m_should_blit_to_screen) {
        command_buffer.Blit(RenderTargetId::Default(), m_target_render_texture->GetRenderTargetId());
      } else {
        command_buffer.SetRenderTarget(RenderTargetId::Default());
      }
      render_frame->ExecuteCommandBuffer(command_buffer);
    }
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::RenderCamera(RenderFrame *render_frame, const RenderFrameContextCamera *camera) {
    CullingParameters culling_parameters;
    culling_parameters.matrix = camera->view_projection_matrix;
    culling_parameters.mask = camera->culling_mask;
    CullingResults culling_results = render_frame->Cull(culling_parameters);

    render_frame->SetCamera(camera->index);
    {
      RenderCommandBuffer command_buffer;
      command_buffer.ClearRenderTarget(ClearFlags::All, camera->background_color);
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    DrawingParameters drawing_parameters_opaque;
    drawing_parameters_opaque.filter_mask = LayerMask::Everything;
    drawing_parameters_opaque.per_object_data = PerObjectData::LightIndices;
    drawing_parameters_opaque.render_order = ShaderRenderOrder::Opaque;
    drawing_parameters_opaque.sorting_settings.camera_position = camera->position;
    drawing_parameters_opaque.sorting_settings.criteria = SortingCriteria::Opaque;
    render_frame->DrawMeshes(culling_results, drawing_parameters_opaque);

    DrawingParameters drawing_parameters_transparent;
    drawing_parameters_transparent.filter_mask = LayerMask::Everything;
    drawing_parameters_transparent.per_object_data = PerObjectData::Nothing;
    drawing_parameters_transparent.render_order = ShaderRenderOrder::Transparent;
    drawing_parameters_transparent.sorting_settings.camera_position = camera->position;
    drawing_parameters_transparent.sorting_settings.criteria = SortingCriteria::Transparent;
    render_frame->DrawMeshes(culling_results, drawing_parameters_transparent);

    render_frame->DrawGizmos();
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::Shutdown() { }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::SetRenderTargetSize(uint32 width, uint32 height) {
    m_render_target_width = width;
    m_render_target_height = height;
  }

}
