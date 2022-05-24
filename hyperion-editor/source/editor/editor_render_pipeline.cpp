//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/math/math.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>
#include <hyperion/ui/ui_element.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/editor_ui.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  //--------------------------------------------------------------
  EditorRenderPipeline::EditorRenderPipeline() {
    m_wrapped_pipeline = new ForwardRenderPipeline();
    SetShouldBlitToScreen(false);
    SetShouldResizeToScreen(false);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Initialize() {
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
        { RenderTextureFormat::UInt32, render_texture_attributes, true },
        { RenderTextureFormat::Depth24Stencil8, render_texture_attributes, false },
      };
      m_object_ids_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
    }
    {
      RenderTextureParameters render_texture_parameters;
      render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
      render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
      render_texture_parameters.attachments = {
        { RenderTextureFormat::RGBA32, render_texture_attributes, true },
        { RenderTextureFormat::Depth24Stencil8, render_texture_attributes, false },
      };
      m_editor_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
    }
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Render(RenderFrame *render_frame, const Array<const RenderFrameContextCamera *> cameras) {
    UpdateSize();
    m_object_ids_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
    m_editor_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());

    m_wrapped_pipeline->Render(render_frame, cameras);

    RenderEditor(render_frame);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderCamera(RenderFrame *render_frame, const RenderFrameContextCamera *camera) {
    m_wrapped_pipeline->RenderCamera(render_frame, camera);
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::Shutdown() {
    m_wrapped_pipeline->Shutdown();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::RenderEditor(RenderFrame *render_frame) {
    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(m_editor_render_texture->GetRenderTargetId());
      command_buffer.ClearRenderTarget(ClearFlags::All, Color::Black());
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    RenderFrameContextCamera editor_camera = EditorCamera::GetContextCamera();
    editor_camera.index = static_cast<uint32>(RenderEngine::GetMainRenderFrame()->GetContext().GetCameras().GetLength());
    RenderEngine::GetMainRenderFrame()->GetContext().AddCamera() = editor_camera;
    m_wrapped_pipeline->RenderCamera(render_frame, &editor_camera);

    {
      RenderCommandBuffer command_buffer;
      command_buffer.SetRenderTarget(RenderTargetId::Default());
      render_frame->ExecuteCommandBuffer(command_buffer);
    }

    if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
      Vector2 mouse_position = Input::GetMousePosition().ToFloat();
      Vector2 ui_space_point = UI::UIElement::ScreenPointToUISpacePoint(mouse_position);

      UI::UIElement *preview_element = EditorUI::GetPreviewElement();
      if (preview_element && preview_element->ContainsScreenPoint(mouse_position)) {
        render_frame->DrawObjectIds(m_object_ids_render_texture->GetRenderTargetId());

        Rect rect = EditorUI::GetPreviewElement()->GetWorldRect();
        Vector2 point = ui_space_point;
        point.x -= rect.x;
        point.y -= rect.y;
        point.x = Math::Clamp(point.x, 0.0f, rect.width - 1);
        point.y = Math::Clamp(point.y, 0.0f, rect.height - 1);

        RectInt region;
        region.position = Vector2Int(static_cast<int32>(point.x), static_cast<int32>(point.y));
        region.size = Vector2Int(1, 1);

        RenderCommandBuffer command_buffer;
        command_buffer.RequestAsyncReadback(m_object_ids_render_texture->GetRenderTargetId(), 0, region, [](auto &result) {
          const uint32 *data = reinterpret_cast<const uint32 *>(result.data.GetData());
          if (result.data.GetLength() >= 4) {
            uint32 id = *data;
            if (id == UINT32_MAX) {
              EditorSelection::Select(EntityId::EMPTY);
            } else {
              EditorSelection::Select(EntityUtilities::CreateFromSmallId(id));
            }
          }
        });
        render_frame->ExecuteCommandBuffer(command_buffer);
      }
    }

    render_frame->DrawEditorUI();
  }

  //--------------------------------------------------------------
  void EditorRenderPipeline::UpdateSize() {
    RectInt preview_rect = EditorUI::GetPreviewRect();
    SetRenderTargetSize(preview_rect.width, preview_rect.height);
  }

}
