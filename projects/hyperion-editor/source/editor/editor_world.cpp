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

        for (float32 x = 0; x < 10; x++) {
            for (float32 z = 0; z < 10; z++) {
                Entity::CreatePrimitive(EntityPrimitive::Cube, Vec3(x * 2, 0, z * 2));
            }
        }
    }

    void EditorWorld::Update(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
    }

}