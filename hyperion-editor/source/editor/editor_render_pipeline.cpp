//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/math/math.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/render_frame.hpp>
#include <hyperion/render/pipeline/forward/forward_render_pipeline.hpp>

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
  void EditorRenderPipeline::Render(RenderPipelineContext &context, const Array<const RenderObjectContextCamera *> &cameras) {
    UpdateSize();
    m_object_ids_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
    m_editor_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());

    SetupRendering(context);
    
    if (EditorUI::GetViewMode() == EditorViewMode::Game) {
      m_wrapped_pipeline->Render(context, cameras);
    }
    
    RenderEditor(context);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Shutdown() {
    m_wrapped_pipeline->Shutdown();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::SetupRendering(RenderPipelineContext &context) {
    m_wrapped_pipeline->SetupRendering(context);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderCamera(RenderPipelineContext &context, const RenderObjectContextCamera *camera, RenderTexture *target_texture) {
    m_wrapped_pipeline->RenderCamera(context, camera, target_texture);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderEditor(RenderPipelineContext &context) {
    if (EditorUI::GetViewMode() == EditorViewMode::Editor) {
      RenderObjectContextCamera editor_camera = EditorCamera::GetObjectContextCamera();
      editor_camera.index = static_cast<uint32>(context.GetRenderFrame()->GetObjectContext().GetCameras().GetLength());
      context.GetRenderFrame()->GetObjectContext().AddCamera() = editor_camera;
      
      SetShouldDrawGizmos(true);
      m_wrapped_pipeline->RenderCamera(context, &editor_camera, m_editor_render_texture);
      SetShouldDrawGizmos(false);

      EditorUI::HandleMouseSelection(context, m_object_ids_render_texture);
    }

    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(RenderTargetId::Default());
      context.ExecuteCommandBuffer(command_buffer);
    }
    
    context.DrawEditorUI();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::UpdateSize() {
    Rect preview_rect = EditorUI::GetPreviewRect();
    SetRenderTargetSize(static_cast<uint32>(preview_rect.width), static_cast<uint32>(preview_rect.height));
  }

}
