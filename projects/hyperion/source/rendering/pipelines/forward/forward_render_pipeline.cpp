//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/io/image_loader.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"
#include "hyperion/entity/components/ui/widget.hpp"
#include "hyperion/rendering/render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    ForwardRenderPipeline::ForwardRenderPipeline(bool is_primary) {
        m_is_primary = is_primary;
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(IRenderDriver *render_driver) {
        if (m_is_primary) {
            Vector<RenderTextureAttachment> attachments = {
                { RenderTextureFormat::RGBA32, TextureParameters() },
                { RenderTextureFormat::Depth24Stencil8, TextureParameters() },
            };
            m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(IRenderDriver *render_driver) {
        render_driver->SetRenderTexture(0);
        render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());

        Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
        render_driver->SetViewport(viewport);

        m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());
        render_driver->SetRenderTexture(m_render_texture->GetResourceId());
        render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());

        World *world = WorldManager::GetActiveWorld();
        RenderWorld(render_driver, world, world->FindComponentOfType<Camera>()->GetCameraData());
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown(IRenderDriver *render_driver) {
        Object::Destroy(m_render_texture);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderWorld(IRenderDriver *render_driver, World *world, const CameraData &camera_data) {
        HYP_ASSERT(world);

        render_driver->SetCameraData(camera_data);

        Vector<MeshRenderer *> mesh_renderers = world->GetMeshRenderers();
        WorldEnvironment world_environment = world->GetEnvironment();

        Set<Material *> materials;
        for (MeshRenderer *mesh_renderer : mesh_renderers) {
            materials.insert(mesh_renderer->GetMaterial());
        }
        Vec3 ambient_color = world_environment.ambient_light.intensity * world_environment.ambient_light.color;
        for (Material *material : materials) {
            if (material->GetShader()->GetAttributes().light_mode == ShaderLightMode::Forward) {
                material->SetVec3("u_lighting.ambient_color", ambient_color);
                material->SetFloat32("u_lighting.main_light.intensity", 0.75f);
                material->SetVec4("u_lighting.main_light.color", Color::White());
                material->SetVec3("u_lighting.main_light.direction", Vec3::Forward());
            }
        }

        for (MeshRenderer *mesh_renderer : mesh_renderers) {
            Transform *transform = mesh_renderer->GetEntity()->GetTransform();
            ResourceId mesh_id = mesh_renderer->GetMesh()->GetResourceId();
            ResourceId material_id = mesh_renderer->GetMaterial()->GetResourceId();
            render_driver->DrawMesh(mesh_id, transform->GetLocalToWorldMatrix(), material_id, 0);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderCanvas(IRenderDriver *render_driver, Canvas *canvas) {
        Vector<Widget *> widgets = canvas->GetEntity()->GetComponentsInChildren<Widget>();

        for (Widget *widget : widgets) {
            WidgetRenderer *widget_renderer = widget->GetEntity()->GetComponent<WidgetRenderer>();
        }
    }

}