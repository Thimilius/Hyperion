#include "hyppch.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/editor/editor_layer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/assets/asset_library.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorLayer::OnAttach() {
        m_camera = Camera::Create();
        m_camera_controller = EditorCameraController(m_camera);

        // Initialize grid vertex buffer
        {
            Color grid_color = Color::White();

            s32 half_grid_size = m_grid_size / 2;
            f32 to_point = (f32)half_grid_size;

            m_grid_vertex_count = (m_grid_size + 1) * 4;
            Vector<VertexImmediate> verticies(m_grid_vertex_count);

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

            Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((u8*)verticies.data(), m_grid_vertex_count * sizeof(VertexImmediate));
            vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
            m_grid_vertex_array = VertexArray::Create();
            m_grid_vertex_array->AddVertexBuffer(vertex_buffer);
        }

        UpdateTitle();
    }

    void EditorLayer::OnUpdate(f32 delta_time) {
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
            m_grid_enabled = !m_grid_enabled;
        }
        if (Input::GetKeyDown(KeyCode::F4)) {
            m_origin_enabled = !m_origin_enabled;
        }

        m_camera_controller.Update(delta_time);
    }

    void EditorLayer::OnRender() {
        ImmediateRenderer::Begin(m_camera);
        {
            if (m_grid_enabled) {
                ImmediateRenderer::Draw(PrimitiveType::Lines, m_grid_vertex_array, m_grid_vertex_count);
            }
            if (m_origin_enabled) {
                // We want to draw the origin on top of the grid
                RenderEngine::Clear(ClearMask::Depth);

                ImmediateRenderer::DrawLine(Vec3(-1000, 0, 0), Vec3(1000, 0, 0), Color::Red());
                ImmediateRenderer::DrawLine(Vec3(0, -1000, 0), Vec3(0, 1000, 0), Color::Green());
                ImmediateRenderer::DrawLine(Vec3(0, 0, -1000), Vec3(0, 0, 1000), Color::Blue());
            }
        }
        ImmediateRenderer::End();
    }

    void EditorLayer::OnTick() {
        UpdateTitle();
    }

    void EditorLayer::UpdateTitle() {
        Window *window = Application::GetInstance()->GetWindow();
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), window->GetVSyncMode() != VSyncMode::DontSync);
        window->SetTitle(title);
    }

}