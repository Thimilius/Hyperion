//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    EditorRenderPipeline::EditorRenderPipeline() {
        m_wrapped_pipeline = new ForwardRenderPipeline();
        m_wrapped_pipeline->SetShouldBlitToScreen(false);
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Initialize() {
        m_wrapped_pipeline->Initialize();

        TextureAttributes render_texture_attributes;
        render_texture_attributes.filter = TextureFilter::Point;
        render_texture_attributes.use_mipmaps = false;

        RenderTextureParameters render_texture_parameters;
        render_texture_parameters.width = m_wrapped_pipeline->GetRenderTargetWidth();
        render_texture_parameters.height = m_wrapped_pipeline->GetRenderTargetHeight();
        render_texture_parameters.attachments = {
            { RenderTextureFormat::UInt32, render_texture_attributes },
            { RenderTextureFormat::Depth24Stencil8, render_texture_attributes },
        };

        m_object_ids_render_texture = AssetManager::CreateRenderTexture(render_texture_parameters);
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Render(RenderFrame *render_frame) {
        m_wrapped_pipeline->Render(render_frame);
        
        if (m_object_ids_render_texture->GetWidth() != GetRenderTargetWidth() || m_object_ids_render_texture->GetHeight() != GetRenderTargetHeight()) {
            m_object_ids_render_texture->Resize(GetRenderTargetWidth(), GetRenderTargetHeight());
        }

        render_frame->DrawObjectIds(m_object_ids_render_texture->GetRenderTargetId());

        RectInt region;
        region.position = Input::GetMousePosition();
        region.size = Vector2Int(1, 1);
        RenderFrameCommandBuffer command_buffer;
        command_buffer.RequestAsyncReadback(m_object_ids_render_texture->GetRenderTargetId(), 0, region, [](const AsyncRequestResult &result) {
            const uint32 *data = reinterpret_cast<const uint32 *>(result.data.GetData());
            if (result.data.GetLength() >= 4) {
                uint32 id = *data;
                HYP_TRACE("ID: {}", id);
            }
        });

        render_frame->DrawEditorUI();
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Shutdown() {
        m_wrapped_pipeline->Shutdown();
    }

}