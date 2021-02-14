//--------------------- Definition Include ---------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/physics/box_collider.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view_grid.hpp"
#include "hyperion/editor/world_view/editor_world_view_camera_controller.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorWorldView::Initialize() {
        s_editor_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(s_editor_world);

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        s_editor_camera = entity->GetComponent<Camera>();
        s_editor_camera_controller = entity->AddComponent<EditorLookAroundCameraController>();

        EditorWorldViewGrid::Initialize();
    }

    //--------------------------------------------------------------
    void EditorWorldView::Update(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            s_should_draw_physics_debug = !s_should_draw_physics_debug;
        }

        s_editor_camera_controller->OnUpdate(delta_time);
    }

    //--------------------------------------------------------------
    void EditorWorldView::Render(IRenderDriver *render_driver) {
        render_driver->SetCameraData(s_editor_camera->GetCameraData());

        {
            Vector<MeshRenderer *> mesh_renderers = s_editor_world->GetMeshRenderers();
            WorldEnvironment world_environment = s_editor_world->GetEnvironment();

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

        if (s_should_draw_physics_debug) {
            ImmediateRenderer::Begin(MeshTopology::Lines);
            WorldManager::GetActiveWorld()->GetPhysicsWorld()->DebugDraw();
            ImmediateRenderer::End();
        }
        
        if (s_should_draw_grid) {
            EditorWorldViewGrid::Render(render_driver, s_editor_camera_controller->GetTargetPosition());
        }
    }

}
