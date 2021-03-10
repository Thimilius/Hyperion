//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/io/image_loader.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"
#include "hyperion/entity/components/ui/widget.hpp"
#include "hyperion/entity/components/ui/text.hpp"
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
                { RenderTextureFormat::RGBA32, TextureParameters::ForRenderTextures() },
                { RenderTextureFormat::Depth24Stencil8, TextureParameters::ForRenderTextures() },
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
        RenderWorld(render_driver, world, world->FindComponentOfType<Camera>()->GetData());
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

        // First we do some global material setup.
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

        // Now we can draw all mesh renderers, meaning a single draw call for each sub mesh.
        for (MeshRenderer *mesh_renderer : mesh_renderers) {
            Transform *transform = mesh_renderer->GetTransform();
            Mat4 local_to_world_matrix = transform->GetLocalToWorldMatrix();

            Mesh *mesh = mesh_renderer->GetMesh();
            ResourceId mesh_id = mesh->GetResourceId();
            uint32 sub_mesh_count = mesh->GetSubMeshCount();

            for (uint32 i = 0; i < sub_mesh_count; i++) {
                Material *material = mesh_renderer->GetMaterial();
                ResourceId material_id = material->GetResourceId();

#ifdef HYP_EDITOR
                material->SetUInt32("u_entity_id", static_cast<uint32>(mesh_renderer->GetEntity()->GetId()));
#endif

                render_driver->DrawMesh(mesh_id, local_to_world_matrix, material_id, i);
            }
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderCanvas(IRenderDriver *render_driver, Canvas *canvas) {
        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        rasterizer_state.depth_mask_enabled = false;
        render_driver->SetRasterizerState(rasterizer_state);

        SetUICameraData(render_driver);

        Vector<Widget *> widgets = canvas->GetEntity()->GetComponentsInChildren<Widget>();
        for (Widget *widget : widgets) {
            WidgetRenderer *widget_renderer = widget->GetWidgetRenderer();

            Mesh *mesh = widget_renderer->GetMesh();
            if (!mesh) {
                continue;
            }
            ResourceId mesh_id = mesh->GetResourceId();

            Material *material = widget_renderer->GetMaterial();
            if (!material) {
                // TODO: Use fallback material.
                continue;
            }
            ResourceId material_id = material->GetResourceId();

            if (widget->GetType() == Type::get<Text>()) {
                Text *text = static_cast<Text *>(widget);
                Font *font = text->GetFont();
                if (!font) {
                    continue;
                }

                material->SetTexture("u_texture", font->GetTexture());
            }
            material->SetVec4("u_color", widget_renderer->GetColor());

            // The widgets already provide their meshes in screen space, meaning they do not need a model matrix.
            render_driver->DrawMesh(mesh_id, Mat4::Identity(), material_id, 0);
        }

        rasterizer_state.depth_mask_enabled = false;
        render_driver->SetRasterizerState(rasterizer_state);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::SetUICameraData(IRenderDriver *render_driver) {
        float32 width = static_cast<float32>(Display::GetWidth());
        float32 height = static_cast<float32>(Display::GetHeight());

        CameraData camera_data = { };
        camera_data.projection_mode = CameraProjectionMode::Orthographic;

        camera_data.view_matrix = Mat4::Identity();
        camera_data.inverse_view_matrix = camera_data.view_matrix.Inverted();

        camera_data.projection_matrix = Mat4::Orthographic(0.0f, width, 0.0f, height, -1.0f, 1.0f);
        camera_data.inverse_projection_matrix = camera_data.projection_matrix.Inverted();

        camera_data.view_projection_matrix = camera_data.projection_matrix;
        camera_data.inverse_view_projection_matrix = camera_data.view_projection_matrix.Inverted();

        render_driver->SetCameraData(camera_data);
    }

}