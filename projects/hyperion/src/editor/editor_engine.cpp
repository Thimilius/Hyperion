#include "hyppch.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/editor/editor_engine.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorEngine::OnInit(const Ref<Camera> &camera) {
        s_camera = camera;
        s_camera_controller = std::make_shared<CameraController>(camera);

        s_gizmos.Init();

        UpdateTitle();
    }

    void EditorEngine::OnUpdate(const Ref<Camera> &camera) {
        Window *window = Application::GetInstance()->GetWindow();

        if (Input::GetKeyDown(KeyCode::Escape) || ((Input::GetKey(KeyCode::LeftControl) || Input::GetKey(KeyCode::RightControl)) && Input::GetKeyDown(KeyCode::W))) {
            Application::GetInstance()->Exit();
        }

        if (Input::GetKeyDown(KeyCode::F1)) {
            window->SetWindowMode(window->GetWindowMode() == WindowMode::Windowed ? WindowMode::Borderless : WindowMode::Windowed);
        }
        if (Input::GetKeyDown(KeyCode::F2)) {
            window->SetVSyncMode(window->GetVSyncMode() == VSyncMode::DontSync ? VSyncMode::EveryVBlank : VSyncMode::DontSync);
            UpdateTitle();
        }
        if (Input::GetKeyDown(KeyCode::F3)) {
            s_gizmos_enabled = !s_gizmos_enabled;
        }

        s_camera_controller->Update(Time::GetDeltaTime());

        if (s_gizmos_enabled) {
            s_gizmos.Update(camera);
        }
    }

    void EditorEngine::OnRender(const Ref<Camera> &camera) {
        // We draw the editor on top of everything so erase depth information
        RenderEngine::Clear(ClearMask::Depth);

        if (s_gizmos_enabled) {
            s_gizmos.Render(camera);
        }
    }

    void EditorEngine::OnTick() {
        UpdateTitle();
    }

    void EditorEngine::UpdateTitle() {
        Window *window = Application::GetInstance()->GetWindow();
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), window->GetVSyncMode() != VSyncMode::DontSync);
        window->SetTitle(title);
    }

}