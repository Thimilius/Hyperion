#include "hyperion/editor/editor_render_pipeline.hpp"

#include <hyperion/core/math/math.hpp>
#include <hyperion/core/image.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/image_loader.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/components/rendering/mesh_renderer.hpp>
#include <hyperion/rendering/render_driver.hpp>

#include "hyperion/editor/editor_world_grid.hpp"

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

        EditorWorldGrid::Initialize();
    }

    void EditorRenderPipeline::Render(IRenderDriver *render_driver, const RenderPipelineContext &context) {
        const CameraData &camera_data = context.GetCameraData();
        {
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
            EditorWorldGrid::Render(render_driver, camera_data);
        }

        {
            RectInt destination_region = RectInt(0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()));
            RectInt source_region = RectInt(0, 0, static_cast<int32>(m_render_texture->GetWidth()), static_cast<int32>(m_render_texture->GetHeight()));
            render_driver->BlitRenderTexture(0, destination_region, m_render_texture->GetResourceId(), source_region);
        }
    }

    void EditorRenderPipeline::Shutdown(IRenderDriver *render_driver) {

    }

}