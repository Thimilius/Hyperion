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
#include <hyperion/rendering/immediate_renderer.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorRenderPipeline::Initialize(IRenderDriver *render_driver) {
        m_forward_render_pipeline = new ForwardRenderPipeline(false);

        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        render_driver->SetRasterizerState(rasterizer_state);

        Vector<RenderTextureAttachment> attachments = {
            { RenderTextureFormat::RGBA32, TextureParameters() },
            { RenderTextureFormat::Depth24Stencil8, TextureParameters() },
        };
        m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);

        m_font = FontLoader::LoadFont("data/fonts/consola.ttf", 32, FontCharacterSet::LatinSupplement);
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Render(IRenderDriver *render_driver) {
        {
            render_driver->SetRenderTexture(0);
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());
            
            Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
            render_driver->SetViewport(viewport);

            m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());
            render_driver->SetRenderTexture(m_render_texture->GetResourceId());
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());

            // We just render the world in the 'normal' way.
            m_forward_render_pipeline->RenderWorld(render_driver, EditorWorldView::GetWorld(), EditorWorldView::GetCamera()->GetCameraData());
        }
        
        {
            EditorWorldView::Render(render_driver);
        }

        {
            Vec2 screen_point = EditorWorldView::GetCamera()->WorldToScreenPoint(Vec3(0, 0, 0));
            float32 half_width = static_cast<float32>(Display::GetWidth()) / 2.0f;
            float32 half_height = static_cast<float32>(Display::GetHeight()) / 2.0f;
            screen_point.x -= half_width;
            screen_point.y -= half_height;

            ImmediateRenderer::DrawText(m_font, "Hello there!\nA new line with a\ttab!", screen_point, 1.0f, Color::White());
        }

        {
            render_driver->BlitRenderTexture(0, m_render_texture->GetResourceId());
        }
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Shutdown(IRenderDriver *render_driver) {
        delete m_forward_render_pipeline;
    }

}