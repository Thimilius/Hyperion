//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/ecs/component/components/core_components.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>
#include <hyperion/ecs/system/systems/render_systems.hpp>
#include <hyperion/ecs/world/world.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_style.hpp"

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
        SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - EditorStyle::HEADER_SIZE);
        m_wrapped_pipeline->Initialize();

        TextureAttributes render_texture_attributes;
        render_texture_attributes.filter = TextureFilter::Point;
        render_texture_attributes.use_mipmaps = false;

        {
            RenderTextureParameters render_texture_parameters;
            render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
            render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
            render_texture_parameters.attachments = {
                { RenderTextureFormat::UInt32, render_texture_attributes },
                { RenderTextureFormat::Depth24Stencil8, render_texture_attributes },
            };
            m_object_ids_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
        }
        {
            RenderTextureParameters render_texture_parameters;
            render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
            render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
            render_texture_parameters.attachments = {
                { RenderTextureFormat::RGBA32, render_texture_attributes },
                { RenderTextureFormat::Depth24Stencil8, render_texture_attributes },
            };
            m_editor_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
        }
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Render(RenderFrame *render_frame, const Array<const RenderFrameContextCamera *> cameras) {
        if (Display::HasChangedSize()) {
            SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - EditorStyle::HEADER_SIZE);
            m_object_ids_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
            m_editor_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
        }

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
            RenderFrameCommandBuffer command_buffer;
            command_buffer.SetRenderTarget(m_editor_render_texture->GetRenderTargetId());
            command_buffer.ClearRenderTarget(ClearFlags::All, Color::Black());
            render_frame->ExecuteCommandBuffer(command_buffer);
        }

        RenderFrameContextCamera editor_camera = EditorCamera::GetContextCamera();
        editor_camera.index = static_cast<uint32>(RenderEngine::GetMainRenderFrame()->GetContext().GetCameras().GetLength());
        RenderEngine::GetMainRenderFrame()->GetContext().AddCamera() = editor_camera;
        m_wrapped_pipeline->RenderCamera(render_frame, &editor_camera);

        {
            RenderFrameCommandBuffer command_buffer;
            command_buffer.SetRenderTarget(RenderTargetId::Default());
            render_frame->ExecuteCommandBuffer(command_buffer);
        }

        Vector2Int mouse_position = Input::GetMousePosition();
        int32 x = 0;
        int32 y = 0;
        int32 width = GetRenderTargetWidth();
        int32 height = GetRenderTargetHeight();
        if (mouse_position.x >= x && mouse_position.x < width && mouse_position.y >= y && mouse_position.y < height) {
            render_frame->DrawObjectIds(m_object_ids_render_texture->GetRenderTargetId());

            RectInt region;
            region.position = mouse_position;
            region.size = Vector2Int(1, 1);
            RenderFrameCommandBuffer command_buffer;
            command_buffer.RequestAsyncReadback(m_object_ids_render_texture->GetRenderTargetId(), 0, region, [](const AsyncRequestResult &result) {
                const uint32 *data = reinterpret_cast<const uint32 *>(result.data.GetData());
                if (result.data.GetLength() >= 4) {
                    uint32 id = *data;
                }
                });
            render_frame->ExecuteCommandBuffer(command_buffer);
        }
        render_frame->DrawEditorUI();
    }
    
}