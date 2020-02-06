#include "hyppch.hpp"

#include "hyperion/editor/editor_engine.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/assets/asset_library.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorEngine::Init() {
        s_camera = Camera::Create();
        s_camera_controller = EditorCameraController(s_camera);

        // Initialize grid vertex buffer
        {
            Color grid_color = Color::White();

            s32 half_grid_size = s_grid_size / 2;
            f32 to_point = (f32)half_grid_size;

            s_grid_vertex_count = (s_grid_size + 1) * 4;
            Vector<VertexImmediate> verticies(s_grid_vertex_count);

            u32 index = 0;
            for (s32 x = -half_grid_size; x <= half_grid_size; x++) {
                if (x == 0) continue;

                f32 from_point = (f32)x;
                verticies[index++] = { Vec3(from_point, 0, to_point), grid_color };
                verticies[index++] = { Vec3(from_point, 0, -to_point), grid_color };
            }
            for (s32 z = -half_grid_size; z <= half_grid_size; z++) {
                if (z == 0) continue;

                f32 from_point = (f32)z;
                verticies[index++] = { Vec3(to_point, 0, from_point), grid_color };
                verticies[index++] = { Vec3(-to_point, 0, from_point), grid_color };
            }

            Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((u8*)verticies.data(), s_grid_vertex_count * sizeof(VertexImmediate));
            vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
            s_grid_vertex_array = VertexArray::Create();
            s_grid_vertex_array->AddVertexBuffer(vertex_buffer);
        }

        UpdateTitle();
    }

    void EditorEngine::Update(f32 delta_time) {
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
            s_overlay_enabled = !s_overlay_enabled;
        }

        s_camera_controller.Update(delta_time);
    }

    void EditorEngine::Render() {
        if (s_overlay_enabled) {
            ImmediateRenderer::Begin(s_camera);
            {
                ImmediateRenderer::Draw(MeshTopology::Lines, s_grid_vertex_array, s_grid_vertex_count);

                // We want to draw the origin on top of the grid
                RenderEngine::Clear(ClearMask::Depth);
                RenderEngine::GetRasterizerState()->SetDepthEquation(DepthEquation::LessEqual);

                ImmediateRenderer::DrawLine(Vec3(-1000, 0, 0), Vec3(1000, 0, 0), Color::Red());
                ImmediateRenderer::DrawLine(Vec3(0, -1000, 0), Vec3(0, 1000, 0), Color::Green());
                ImmediateRenderer::DrawLine(Vec3(0, 0, -1000), Vec3(0, 0, 1000), Color::Blue());
            }
            ImmediateRenderer::End();
        }
    }

    void EditorEngine::Tick() {
        UpdateTitle();
    }

    void EditorEngine::UpdateTitle() {
        Window *window = Application::GetInstance()->GetWindow();
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), window->GetVSyncMode() != VSyncMode::DontSync);
        window->SetTitle(title);
    }

}