//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/math/math.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_ui.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  //--------------------------------------------------------------
  EditorRenderPipeline::EditorRenderPipeline() {
    m_wrapped_pipeline = new ForwardRenderPipeline();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Initialize() {
    SetShouldBlitToScreen(false);
    SetShouldResizeToScreen(false);
    SetShouldDrawGizmos(false);
    SetShouldDoSetup(false);

    UpdateSize();
    m_wrapped_pipeline->Initialize();

    TextureAttributes render_texture_attributes;
    render_texture_attributes.filter = TextureFilter::Point;
    render_texture_attributes.use_mipmaps = false;

    {
      RenderTextureParameters render_texture_parameters;
      render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
      render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
      render_texture_parameters.attachments = {
        { RenderTextureFormat::RG32UInt, render_texture_attributes, true },
        { RenderTextureFormat::Depth24Stencil8, render_texture_attributes, false },
      };
      m_object_ids_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
    }
    {
      RenderTextureParameters render_texture_parameters;
      render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
      render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
      render_texture_parameters.attachments = {
        { RenderTextureFormat::RGBA8, render_texture_attributes, true },
        { RenderTextureFormat::Depth24Stencil8, render_texture_attributes, false },
      };
      m_editor_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
    }
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Render(RenderFrame *render_frame, const Array<const RenderFrameContextCamera *> &cameras) {
    UpdateSize();
    m_object_ids_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
    m_editor_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());

    SetupRendering(render_frame);
    
    if (EditorUI::GetViewMode() == EditorViewMode::Game) {
      m_wrapped_pipeline->Render(render_frame, cameras);
    }
    
    RenderEditor(render_frame);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Shutdown() {
    m_wrapped_pipeline->Shutdown();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::SetupRendering(Rendering::RenderFrame *render_frame) {
    m_wrapped_pipeline->SetupRendering(render_frame);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderCamera(RenderFrame *render_frame, const RenderFrameContextCamera *camera) {
    m_wrapped_pipeline->RenderCamera(render_frame, camera);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderEditor(RenderFrame *render_frame) {
    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(m_editor_render_texture->GetRenderTargetId());
      command_buffer.ClearRenderTarget(ClearFlags::All, Color::Black());
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    if (EditorUI::GetViewMode() == EditorViewMode::Editor) {
      RenderFrameContextCamera editor_camera = EditorCamera::GetContextCamera();
      editor_camera.index = static_cast<uint32>(RenderEngine::GetMainRenderFrame()->GetContext().GetCameras().GetLength());
      RenderEngine::GetMainRenderFrame()->GetContext().AddCamera() = editor_camera;
      SetShouldDrawGizmos(true);
      m_wrapped_pipeline->RenderCamera(render_frame, &editor_camera);
      SetShouldDrawGizmos(false);  
    }

    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(RenderTargetId::Default());
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    EditorUI::HandleMouseSelection(render_frame, m_object_ids_render_texture);
    
    render_frame->DrawEditorUI();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::UpdateSize() {
    Rect preview_rect = EditorUI::GetPreviewRect();
    SetRenderTargetSize(static_cast<uint32>(preview_rect.width), static_cast<uint32>(preview_rect.height));
  }

}
