//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_world_camera.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_style.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    static World *g_world;

    //--------------------------------------------------------------
    void EditorWorldCamera::Initialize(EntityId camera, World *world) {
        g_world = world;

        s_camera_controller = new LookAroundCameraController(camera);
        s_camera_controller->Reset(world);
    }

    //--------------------------------------------------------------
    void EditorWorldCamera::Update(float32 delta_time, EntityId camera, World *world) {
        s_camera_controller->Update(g_world, delta_time);
        if (Input::IsKeyDown(KeyCode::R)) {
            s_camera_controller->Reset(g_world);
        }

        uint32 render_target_height = Display::GetHeight() - EditorStyle::HEADER_SIZE;
        float32 viewport_clipping_height = (Display::GetHeight() - EditorStyle::HEADER_SIZE) / static_cast<float32>(Display::GetHeight());
        world->GetComponent<Rendering::CameraComponent>(camera)->viewport_clipping.height = viewport_clipping_height;
    }

    //--------------------------------------------------------------
    void EditorWorldCamera::Reset() {
        s_camera_controller->Reset(g_world);
    }

}