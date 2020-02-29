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

        InitGridVertexArray();
        UpdateWindowTitle();
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
            UpdateWindowTitle();
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
            }
            ImmediateRenderer::End();
        }
    }

    void EditorEngine::Tick() {
        UpdateWindowTitle();
    }

    void EditorEngine::UpdateWindowTitle() {
        Window *window = Application::GetInstance()->GetWindow();
        f64 memory = (f64)OperatingSystem::GetInstance()->GetMemoryUsage() / 1024.0 / 1024.0;
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {} | Memory: {:.2f} MiB",
            Time::GetFPS(),
            Time::GetFrameTime(),
            window->GetVSyncMode() != VSyncMode::DontSync,
            memory
        );
        window->SetTitle(title);
    }

    void EditorEngine::InitGridVertexArray() {
        Color default_grid_color = Color(0.25f, 0.25f, 0.25f, 0.25f);
        Color special_grid_color = Color(0.75f, 0.75f, 0.75f, 0.75f);

        s32 half_grid_size = s_grid_size / 2;
        f32 to_point = (f32)half_grid_size;

        s_grid_vertex_count = ((s_grid_size) * 4) + 6;
        Vector<VertexImmediate> verticies(s_grid_vertex_count);

        u32 index = 0;
        for (s32 x = -half_grid_size; x <= half_grid_size; x++) {
            if (x == 0) continue; // Skip center line

            f32 from_point = (f32)x;
            Color m_color = (x % 10) == 0 ? special_grid_color : default_grid_color;
            verticies[index++] = { Vec3(from_point, 0, to_point), m_color };
            verticies[index++] = { Vec3(from_point, 0, -to_point), m_color };
        }
        for (s32 z = -half_grid_size; z <= half_grid_size; z++) {
            if (z == 0) continue; // Skip center line

            f32 from_point = (f32)z;
            Color m_color = (z % 10) == 0 ? special_grid_color : default_grid_color;
            verticies[index++] = { Vec3(to_point, 0, from_point), m_color };
            verticies[index++] = { Vec3(-to_point, 0, from_point), m_color };
        }

        // Axis lines
        verticies[index++] = { Vec3(-1000, 0, 0), Color::Red() };
        verticies[index++] = { Vec3(1000, 0, 0), Color::Red() };
        verticies[index++] = { Vec3(0, -1000, 0), Color::Green() };
        verticies[index++] = { Vec3(0, 1000, 0), Color::Green() };
        verticies[index++] = { Vec3(0, 0, -1000), Color::Blue() };
        verticies[index++] = { Vec3(0, 0, 1000), Color::Blue() };

        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((u8 *)verticies.data(), s_grid_vertex_count * sizeof(VertexImmediate));
        vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
        s_grid_vertex_array = VertexArray::Create();
        s_grid_vertex_array->AddVertexBuffer(vertex_buffer);
    }

}