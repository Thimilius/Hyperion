//--------------------- Definition Include ---------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/math.hpp>
#include <hyperion/core/image.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/image_loader.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/rendering/mesh_renderer.hpp>
#include <hyperion/rendering/render_driver.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorRenderPipeline::Initialize(IRenderDriver *render_driver) {
        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        render_driver->SetRasterizerState(rasterizer_state);

        Vector<RenderTextureAttachment> attachments = {
            { RenderTextureFormat::RGBA32, TextureParameters() },
            { RenderTextureFormat::Depth24Stencil8, TextureParameters() },
        };
        m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Render(IRenderDriver *render_driver) {
        const CameraData &camera_data = EditorWorldView::GetCamera()->GetCameraData();
        {
            render_driver->SetRenderTexture(0);
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());

            
            Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
            render_driver->SetViewport(viewport);

            m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());
            render_driver->SetRenderTexture(m_render_texture->GetResourceId());
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());
        }
        
        {
            EditorWorldView::Render(render_driver);
        }

        {
            render_driver->BlitRenderTexture(0, m_render_texture->GetResourceId());
        }
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Shutdown(IRenderDriver *render_driver) {

    }

}