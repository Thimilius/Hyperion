//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_camera.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_style.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorCamera::Initialize(EntityId camera, World *world) {
        s_camera_controller = new LookAroundCameraController(camera);
        s_camera_controller->Reset(world);
    }

    //--------------------------------------------------------------
    void EditorCamera::Update(float32 delta_time, EntityId camera, World *world) {
        s_camera_controller->Update(world, delta_time);
        if (Input::IsKeyDown(KeyCode::R)) {
            s_camera_controller->Reset(world);
        }

        uint32 render_target_height = Display::GetHeight() - EditorStyle::HEADER_SIZE;
        float32 viewport_clipping_height = (Display::GetHeight() - EditorStyle::HEADER_SIZE) / static_cast<float32>(Display::GetHeight());
        world->GetComponent<Rendering::CameraComponent>(camera)->viewport_clipping.height = viewport_clipping_height;
    }

    //--------------------------------------------------------------
    void EditorCamera::Reset() {
        s_camera_controller->Reset(EditorApplication::GetWorld());
    }

}