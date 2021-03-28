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
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/world_view/editor_world_view.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorRenderPipeline::Initialize(IRenderDriver *render_driver) {
        m_forward_render_pipeline = new ForwardRenderPipeline(false);
        m_forward_render_pipeline->Initialize(render_driver);

        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        render_driver->SetRasterizerState(rasterizer_state);

        Vector<RenderTextureAttachment> attachments = {
            { RenderTextureFormat::RGBA32, TextureParameters::ForRenderTextures() },
            { RenderTextureFormat::UInt32, TextureParameters::ForRenderTextures() },
            { RenderTextureFormat::Depth24Stencil8, TextureParameters::ForRenderTextures() },
        };
        m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);
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

            CameraData camera_data = EditorWorldView::GetCamera()->GetData();

            // We just render the world in the 'normal' way.
            m_forward_render_pipeline->DrawWorld(render_driver, EditorApplication::GetEditingWorld(), camera_data);
            m_forward_render_pipeline->DrawWorld(render_driver, EditorWorldView::GetWorld(), camera_data);
        }
        
        {
            EditorWorldView::Render(render_driver);
        }

        {
            m_forward_render_pipeline->DrawCanvas(render_driver, EditorWorldView::GetCanvas());
        }

        {
            render_driver->SetRasterizerState(RasterizerState());
            render_driver->BlitRenderTexture(0, m_render_texture->GetResourceId());
        }
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::Shutdown(IRenderDriver *render_driver) {
        delete m_forward_render_pipeline;
    }

    //--------------------------------------------------------------
    void EditorRenderPipeline::DrawMeshRenderer(IRenderDriver *render_driver, MeshRenderer *mesh_renderer, Material *overwrite_material) {
        m_forward_render_pipeline->DrawMeshRenderer(render_driver, mesh_renderer, overwrite_material);
    }

}