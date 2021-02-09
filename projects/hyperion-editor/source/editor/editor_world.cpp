#include "hyperion/editor/editor_world.hpp"

#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/physics/box_collider.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>

#include "hyperion/editor/editor_first_person_camera_controller.hpp"
#include "hyperion/editor/editor_look_around_camera_controller.hpp"
#include "hyperion/editor/editor_world_grid.hpp"

namespace Hyperion::Editor {

    void EditorWorld::Initialize() {
        s_editor_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(s_editor_world);

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        entity->AddComponent<EditorLookAroundCameraController>();

        EditorWorldGrid::Initialize();
    }

    void EditorWorld::Update(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            s_should_draw_physics_debug = !s_should_draw_physics_debug;
        }
    }

    void EditorWorld::Render(Rendering::IRenderDriver *render_driver, const Rendering::CameraData &camera_data) {
        if (s_should_draw_physics_debug) {
            Rendering::ImmediateRenderer::Begin(Rendering::MeshTopology::Lines);
            WorldManager::GetActiveWorld()->GetPhysicsWorld()->DebugDraw();
            Rendering::ImmediateRenderer::End();
        }
        
        if (s_should_draw_grid) {
            EditorWorldGrid::Render(render_driver, camera_data);
        }
    }

}
