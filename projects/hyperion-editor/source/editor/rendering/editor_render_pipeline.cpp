#include "hyperion/editor/rendering/editor_render_pipeline.hpp"

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

#include "hyperion/editor/world/editor_world.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

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

    void EditorRenderPipeline::Render(IRenderDriver *render_driver, const RenderPipelineContext &context) {
        const CameraData &camera_data = context.GetCameraData();
        {
            render_driver->SetRenderTexture(0);
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());

            render_driver->SetCameraData(camera_data);
            
            Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
            render_driver->SetViewport(viewport);

            m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());
            render_driver->SetRenderTexture(m_render_texture->GetResourceId());
            render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());
        }

        {
            Vector<MeshRenderer *> mesh_renderers = context.GetMeshRenderers();
            for (MeshRenderer *mesh_renderer : mesh_renderers) {
                Transform *transform = mesh_renderer->GetEntity()->GetTransform();
                ResourceId mesh_id = mesh_renderer->GetMesh()->GetResourceId();
                ResourceId material_id = mesh_renderer->GetMaterial()->GetResourceId();
                render_driver->DrawMesh(mesh_id, transform->GetLocalToWorldMatrix(), material_id, 0);
            }
        }
        
        {
            EditorWorld::Render(render_driver, camera_data);
        }

        {
            render_driver->BlitRenderTexture(0, m_render_texture->GetResourceId());
        }
    }

    void EditorRenderPipeline::Shutdown(IRenderDriver *render_driver) {

    }

}