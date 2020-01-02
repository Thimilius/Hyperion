#include "hyppch.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/editor/editor_engine.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/assets/asset_library.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorEngine::OnInit(const Ref<Camera> &camera) {
        s_camera = camera;
        s_camera_controller = CameraController::Create(camera);

        // Initialize grid vertex buffer
        {
            Color grid_color = Color::White();

            s32 half_grid_size = s_grid_size / 2;
            f32 to_point = (f32)half_grid_size;

            s_grid_vertex_count = (s_grid_size + 1) * 4;
            Vector<VertexImmediate> verticies(s_grid_vertex_count);

            u32 index = 0;
            for (s32 x = -half_grid_size; x <= half_grid_size; x++) {
                f32 from_point = (f32)x;
                verticies[index++] = { Vec3(from_point, 0, to_point), grid_color };
                verticies[index++] = { Vec3(from_point, 0, -to_point), grid_color };
            }
            for (s32 z = -half_grid_size; z <= half_grid_size; z++) {
                f32 from_point = (f32)z;
                verticies[index++] = { Vec3(to_point, 0, from_point), grid_color };
                verticies[index++] = { Vec3(-to_point, 0, from_point), grid_color };
            }

            Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((u8*)verticies.data(), sizeof(VertexImmediate) * s_grid_vertex_count);
            vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
            s_grid_vertex_array = VertexArray::Create();
            s_grid_vertex_array->AddVertexBuffer(vertex_buffer);
        }

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
        ImmediateRenderer::Begin(camera);
        {
            if (s_grid_enabled) {
                ImmediateRenderer::Draw(PrimitiveType::Lines, s_grid_vertex_array, s_grid_vertex_count);
            }
            if (s_origin_enabled) {
                // We want to draw the origin on top of the grid
                RenderEngine::Clear(ClearMask::Depth);

                ImmediateRenderer::DrawLine(Vec3(-1000, 0, 0), Vec3(1000, 0, 0), Color::Red());
                ImmediateRenderer::DrawLine(Vec3(0, -1000, 0), Vec3(0, 1000, 0), Color::Green());
                ImmediateRenderer::DrawLine(Vec3(0, 0, -1000), Vec3(0, 0, 1000), Color::Blue());
            }
        }
        ImmediateRenderer::End();
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