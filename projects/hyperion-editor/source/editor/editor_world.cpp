#include "hyperion/editor/editor_world.hpp"

#include <hyperion/core/app/input.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

#include "hyperion/editor/editor_first_person_camera_controller.hpp"
#include "hyperion/editor/editor_look_around_camera_controller.hpp"

namespace Hyperion::Editor {

    void EditorWorld::Initialize() {
        World *world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(world);

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        entity->AddComponent<EditorLookAroundCameraController>();

        Entity::CreatePrimitive(EntityPrimitive::Cube);
    }

    void EditorWorld::Update(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
    }

}