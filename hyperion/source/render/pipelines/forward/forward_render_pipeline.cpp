//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void ForwardRenderPipeline::Initialize() {
    if (m_should_resize_to_screen) {
      m_render_target_width = Display::GetWidth();
      m_render_target_height = Display::GetHeight();
    }

    TextureAttributes target_texture_attributes;
    target_texture_attributes.filter = TextureFilter::Point;
    target_texture_attributes.use_mipmaps = false;
    RenderTextureParameters render_texture_parameters;
    render_texture_parameters.width = m_render_target_width;
    render_texture_parameters.height = m_render_target_height;
    render_texture_parameters.attachments = {
      { RenderTextureFormat::RGBA8, target_texture_attributes, true },
      { RenderTextureFormat::Depth24Stencil8, target_texture_attributes, false },
    };
    m_target_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);

    TextureAttributes shadow_texture_attributes;
    shadow_texture_attributes.filter = TextureFilter::Point;
    shadow_texture_attributes.use_mipmaps = false;
    shadow_texture_attributes.wrap_mode = TextureWrapMode::Border;
    shadow_texture_attributes.border_color = Color::White();
    render_texture_parameters.width = SHADOW_MAP_SIZE;
    render_texture_parameters.height = SHADOW_MAP_SIZE;
    render_texture_parameters.attachments = {
      { RenderTextureFormat::Depth24, shadow_texture_attributes, true },
    };
    m_shadow_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::Render(RenderFrame *render_frame, const Array<const RenderFrameContextCamera *> &cameras) {
    if (m_should_do_setup) {
      SetupRendering(render_frame);
    }

    for (const RenderFrameContextCamera *camera : cameras) {
      RenderCamera(render_frame, camera, m_target_render_texture);
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
  void ForwardRenderPipeline::Shutdown() { }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::SetupRendering(RenderFrame *render_frame) {
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
      m_lighting.SetupLighting(render_frame->GetContext(), command_buffer);
      render_frame->ExecuteCommandBuffer(command_buffer);
    }
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::RenderCamera(RenderFrame *render_frame, const RenderFrameContextCamera *camera, RenderTexture *target_texture) {
    CullingParameters culling_parameters;
    culling_parameters.matrix = camera->view_projection_matrix;
    culling_parameters.mask = camera->culling_mask;
    CullingResults culling_results = render_frame->Cull(culling_parameters);

    DrawShadows(render_frame);
    DrawMeshes(render_frame, camera, culling_results, target_texture);
    
    if (m_should_draw_gizmos) {
      render_frame->DrawGizmos();  
    }
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::SetRenderTargetSize(uint32 width, uint32 height) {
    m_render_target_width = width;
    m_render_target_height = height;
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::DrawShadows(RenderFrame *render_frame) {
    const RenderFrameContextLight *main_light = m_lighting.GetMainLight();
    if (main_light == nullptr || main_light->shadows == LightShadows::None) {
      return;
    }
    
    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(m_shadow_render_texture->GetRenderTargetId());
      command_buffer.ClearRenderTarget(ClearFlags::Depth, Color::Black());
      render_frame->ExecuteCommandBuffer(command_buffer);
    }
    
    ShadowParameters shadow_parameters;
    shadow_parameters.light_index = 0;
    shadow_parameters.shadow_map_size = SHADOW_MAP_SIZE;
    shadow_parameters.light_space_matrix = m_lighting.CalculateLightSpaceMatrixForMainLight();
    render_frame->DrawShadows(shadow_parameters);
  }

  //--------------------------------------------------------------
  void ForwardRenderPipeline::DrawMeshes(
    RenderFrame *render_frame,
    const RenderFrameContextCamera *camera,
    CullingResults &culling_results,
    RenderTexture *target_texture) {
    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(target_texture->GetRenderTargetId());
      command_buffer.ClearRenderTarget(ClearFlags::All, camera->background_color);
      render_frame->ExecuteCommandBuffer(command_buffer);
    }
    
    render_frame->SetCamera(camera->index);
    
    DrawingParameters drawing_parameters_opaque;
    drawing_parameters_opaque.filter_mask = LayerMask::Everything;
    drawing_parameters_opaque.per_object_data = PerObjectData::LightIndices;
    drawing_parameters_opaque.render_order = ShaderRenderOrder::Opaque;
    drawing_parameters_opaque.sorting_settings.camera_position = camera->position;
    drawing_parameters_opaque.sorting_settings.criteria = SortingCriteria::Opaque;
    drawing_parameters_opaque.shadow_settings.light_space_matrix = m_lighting.CalculateLightSpaceMatrixForMainLight();
    drawing_parameters_opaque.shadow_settings.shadow_map_render_target_id = m_shadow_render_texture->GetRenderTargetId();
    render_frame->DrawMeshes(culling_results, drawing_parameters_opaque);

    DrawingParameters drawing_parameters_transparent;
    drawing_parameters_transparent.filter_mask = LayerMask::Everything;
    drawing_parameters_transparent.per_object_data = PerObjectData::Nothing;
    drawing_parameters_transparent.render_order = ShaderRenderOrder::Transparent;
    drawing_parameters_transparent.sorting_settings.camera_position = camera->position;
    drawing_parameters_transparent.sorting_settings.criteria = SortingCriteria::Transparent;
    drawing_parameters_transparent.shadow_settings.light_space_matrix = m_lighting.CalculateLightSpaceMatrixForMainLight();
    drawing_parameters_transparent.shadow_settings.shadow_map_render_target_id = m_shadow_render_texture->GetRenderTargetId();
    render_frame->DrawMeshes(culling_results, drawing_parameters_transparent);
  }

}
