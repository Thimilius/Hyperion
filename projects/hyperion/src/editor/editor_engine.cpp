#include "hyppch.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorEngine::OnInit(const Ref<Camera> &camera) {
        s_camera = camera;
        s_camera_controller = CameraController::Create(camera);

        UpdateTitle();
    }

    void EditorEngine::OnUpdate(const Ref<Camera> &camera) {
        Ref<Window> window = Application::GetInstance()->GetWindow();

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
            s_grid_enabled = !s_grid_enabled;
        }
        if (Input::GetKeyDown(KeyCode::F4)) {
            s_origin_enabled = !s_origin_enabled;
        }

        s_camera_controller->Update(Time::GetDeltaTime());
    }

    void EditorEngine::OnRender(const Ref<Camera> &camera) {
        if (s_grid_enabled) {
            // NOTE: The performance of this is obviously far from optimal
            ImmediateRenderer::Begin(camera);
            {
                Color grid_color = Color::White();

                s32 grid_size = 20;
                s32 half_grid_size = grid_size / 2;
                f32 to_point = (f32)half_grid_size;

                for (s32 x = -half_grid_size; x <= half_grid_size; x++) {
                    // We want to skip drawing the middle line to avoid an ugly overlap with the origin lines
                    if (s_origin_enabled && x == 0) {
                        continue;
                    }

                    f32 from_point = (f32)x;
                    ImmediateRenderer::DrawLine(Vec3(from_point, 0, to_point), Vec3(from_point, 0, -to_point), grid_color);
                }
                for (s32 z = -half_grid_size; z <= half_grid_size; z++) {
                    // We want to skip drawing the middle line to avoid an ugly overlap with the origin lines
                    if (s_origin_enabled && z == 0) {
                        continue;
                    }

                    f32 from_point = (f32)z;
                    ImmediateRenderer::DrawLine(Vec3(to_point, 0, from_point), Vec3(-to_point, 0, from_point), grid_color);
                }
            }
            ImmediateRenderer::End();
        }

        if (s_origin_enabled) {
            // We want to draw the origin on top of the grid
            RenderEngine::Clear(ClearMask::Depth);

            ImmediateRenderer::Begin(camera);
            {
                ImmediateRenderer::DrawLine(Vec3(-1000, 0, 0), Vec3(1000, 0, 0), Color::Red());
                ImmediateRenderer::DrawLine(Vec3(0, -1000, 0), Vec3(0, 1000, 0), Color::Green());
                ImmediateRenderer::DrawLine(Vec3(0, 0, -1000), Vec3(0, 0, 1000), Color::Blue());
            }
            ImmediateRenderer::End();
        }
    }

    void EditorEngine::OnTick() {
        UpdateTitle();
    }

    void EditorEngine::UpdateTitle() {
        Ref<Window> window = Application::GetInstance()->GetWindow();
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), window->GetVSyncMode() != VSyncMode::DontSync);
        window->SetTitle(title);
    }

}