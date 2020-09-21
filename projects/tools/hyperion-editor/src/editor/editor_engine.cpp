#include "hyperion/editor/editor_engine.hpp"

#include <hyperion/core/operating_system.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/rendering/render_command.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>
#include <hyperion/rendering/forward_renderer.hpp>
#include <hyperion/rendering/font.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/mesh_factory.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>
#include <hyperion/entity/components/rendering/light.hpp>
#include <hyperion/entity/components/rendering/mesh_renderer.hpp>
#include <hyperion/entity/components/physics/collider.hpp>
#include <hyperion/physics/physics_world.hpp>

#include "hyperion/editor/editor_selection.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorEngine::Init() {
        s_editor_world = WorldManager::CreateWorld();
        s_game_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(s_game_world);

        s_camera = Entity::Create("Camera", Vec3(), Quaternion::Identity(), nullptr, s_editor_world)->AddComponent<Camera>();
        s_camera_controller = EditorCameraController(s_camera);

        s_icon_mesh = MeshFactory::CreateQuad(0.5f, 0.5f);
        s_icon_material = Material::Create(AssetManager::GetShader("standard_unlit_texture"));
        
        s_selection_material = Material::Create(AssetManager::GetShader("standard_unlit"));

        s_font = Font::Create("data/fonts/robotomono_regular.ttf", 16, FontCharacterSet::All);

        s_gizmo = Entity::Create("Gizmo", Vec3::Zero(), Quaternion::Identity(), nullptr, s_editor_world)->AddComponent<EditorGizmo>();
        s_gizmo->SetCamera(s_camera);
        s_gizmo->GetEntity()->SetActive(false);

        InitGridVertexArray();
        UpdateStats();
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
            UpdateStats();
        }
        if (Input::GetKeyDown(KeyCode::F3)) {
            if ((s_overlay_flags & EditorOverlayFlags::Grid) == EditorOverlayFlags::Grid) {
                s_overlay_flags = s_overlay_flags & ~EditorOverlayFlags::Grid;
            } else {
                s_overlay_flags |= EditorOverlayFlags::Grid;
            }
        }
        if (Input::GetKeyDown(KeyCode::F4)) {
            if ((s_overlay_flags & EditorOverlayFlags::Stats) == EditorOverlayFlags::Stats) {
                s_overlay_flags = s_overlay_flags & ~EditorOverlayFlags::Stats;
            } else {
                s_overlay_flags |= EditorOverlayFlags::Stats;
            }
        }
        if (Input::GetKeyDown(KeyCode::F5)) {
            if ((s_overlay_flags & EditorOverlayFlags::PhysicsDebug) == EditorOverlayFlags::PhysicsDebug) {
                s_overlay_flags = s_overlay_flags & ~EditorOverlayFlags::PhysicsDebug;
            } else {
                s_overlay_flags |= EditorOverlayFlags::PhysicsDebug;
            }
        }

        if (Input::GetMouseButtonDown(MouseButtonCode::Left)) {
            Ray ray = s_camera->ScreenPointToRay(Input::GetMousePosition());
            Physics::RaycastResult result;
            if (s_game_world->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
                Entity *entity = result.collider->GetEntity();
                s_gizmo->GetEntity()->SetActive(true);
                s_gizmo->GetTransform()->SetPosition(entity->GetTransform()->GetPosition());
                s_gizmo->SetSelection(entity);
                EditorSelection::SetSelection(entity);
            } else {
                // HACK
                if (!s_editor_world->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
                    s_gizmo->GetEntity()->SetActive(false);
                    s_gizmo->SetSelection(nullptr);
                    EditorSelection::SetSelection(nullptr);
                }
            }
        }

        s_selection_color.a = Math::Cos(static_cast<f32>(Time::GetTime()) * 4.0f) * 0.4f + 0.6f;
        s_selection_material->SetColor("u_color", s_selection_color);

        s_camera_controller.Update(delta_time);
    }

    void EditorEngine::Render() {
        ForwardRenderer::Begin(s_camera->GetCameraData());
        {
            RenderCommand::Clear(ClearMask::Depth);
            ForwardRenderer::DrawEntities(s_game_world);
        }
        ForwardRenderer::End();

        bool blending_enabled = RenderCommand::GetRasterizerState()->IsBlendingEnabled();
        RenderCommand::GetRasterizerState()->SetBlendingEnabled(true);

        if ((s_overlay_flags & EditorOverlayFlags::Grid) == EditorOverlayFlags::Grid) {
            ImmediateRenderer::Begin(s_camera->GetCameraData());
            {
                ImmediateRenderer::DrawWire(MeshTopology::Lines, s_grid_vertex_array, s_grid_vertex_count);
            }
            ImmediateRenderer::End();
        }

        if ((s_overlay_flags & EditorOverlayFlags::PhysicsDebug) == EditorOverlayFlags::PhysicsDebug) {
            RenderCommand::Clear(ClearMask::Depth);

            ImmediateRenderer::Begin(s_camera->GetCameraData(), MeshTopology::Lines);
            {
                s_editor_world->GetPhysicsWorld()->DebugDraw();
            }
            ImmediateRenderer::End();
        }

        if ((s_overlay_flags & EditorOverlayFlags::Lights) == EditorOverlayFlags::Lights) {
            RenderCommand::Clear(ClearMask::Depth);

            ForwardRenderer::Begin(s_camera->GetCameraData());
            {
                // Draw light icons
                {
                    s_icon_material->SetTexture2D("u_texture", AssetManager::GetTexture2D("icon_light"));
                    Mat4 camera_rotation = Mat4::Rotate(s_camera->GetTransform()->GetRotation());
                    Vec3 camera_position = s_camera->GetTransform()->GetPosition();

                    Vector<Light *> lights = s_game_world->GetLights();
                    std::sort(lights.begin(), lights.end(), [camera_position](Light *first, Light *second) {
                        f32 distance_first = (camera_position - first->GetTransform()->GetPosition()).SqrMagnitude();
                        f32 distance_second = (camera_position - second->GetTransform()->GetPosition()).SqrMagnitude();
                        return distance_first > distance_second;
                    });

                    for (Light *light : lights) {
                        Transform *transform = light->GetTransform();
                        Vec3 position = transform->GetPosition();
                        Vec3 forward = transform->GetForward();
                        Mat4 model = Mat4::Translate(position) * camera_rotation;

                        Color color = light->GetColor();
                        color.a = Math::Clamp01((camera_position - position).SqrMagnitude() - 0.15f);
                        s_icon_material->SetColor("u_color", color);

                        ForwardRenderer::DrawMesh(s_icon_mesh, s_icon_material, model);
                    }
                }
            }
            ForwardRenderer::End();
        }

        if ((s_overlay_flags & EditorOverlayFlags::Selection) == EditorOverlayFlags::Selection) {
            ForwardRenderer::Begin(s_camera->GetCameraData());
            {
                PolygonMode polygon_mode = RenderCommand::GetRasterizerState()->GetPolygonMode();
                RenderCommand::GetRasterizerState()->SetPolygonMode(PolygonMode::Line);

                Object *selection = EditorSelection::GetSelection();
                if (selection != nullptr && selection->IsBase(Entity::GetTypeStatic())) {
                    Entity *entity = static_cast<Entity *>(selection);
                    MeshRenderer *renderer = entity->GetComponent<MeshRenderer>();
                    if (renderer != nullptr) {
                        Transform *transform = entity->GetTransform();
                        Mat4 local_to_world = transform->GetLocalToWorldMatrix();
                        Mat4 world_to_local = transform->GetWorldToLocalMatrix();

                        ForwardRenderer::DrawMesh(renderer->GetRenderMesh(), s_selection_material, local_to_world, world_to_local);
                    }
                }

                RenderCommand::GetRasterizerState()->SetPolygonMode(polygon_mode);
            }
        }

        if ((s_overlay_flags & EditorOverlayFlags::Gizmo) == EditorOverlayFlags::Gizmo) {
            ForwardRenderer::Begin(s_camera->GetCameraData());
            {
                bool culling_enabled = RenderCommand::GetRasterizerState()->IsCullingEnabled();
                RenderCommand::GetRasterizerState()->SetCullingEnabled(false);

                RenderCommand::Clear(ClearMask::Depth);
                ForwardRenderer::DrawEntities(s_editor_world);

                RenderCommand::GetRasterizerState()->SetCullingEnabled(culling_enabled);
            }
            ForwardRenderer::End();
        }

        if ((s_overlay_flags & EditorOverlayFlags::Stats) == EditorOverlayFlags::Stats) {
            f32 y = static_cast<f32>(Display::GetHeight() - s_font->GetSize());
            ImmediateRenderer::DrawText(s_stats, s_font, 0, y, 1.0f, Color::White());
        }

        RenderCommand::GetRasterizerState()->SetBlendingEnabled(blending_enabled);
    }

    void EditorEngine::Tick() {
        UpdateStats();
    }

    void EditorEngine::UpdateStats() {
        f64 memory = static_cast<f64>(OperatingSystem::GetInstance()->GetMemoryUsage() / 1024.0 / 1024.0);
        s_stats = StringUtils::Format("FPS: {} ({:.2f} ms) | VSync: {} | Memory: {:.2f} MiB",
            Time::GetFPS(),
            Time::GetFrameTime(),
            Application::GetInstance()->GetWindow()->GetVSyncMode() != VSyncMode::DontSync,
            memory
        );
    }

    void EditorEngine::InitGridVertexArray() {
        Color default_grid_color = Color(0.25f, 0.25f, 0.25f, 0.25f);
        Color special_grid_color = Color(0.75f, 0.75f, 0.75f, 0.75f);

        s32 half_grid_size = s_grid_size / 2;
        f32 to_point = static_cast<f32>(half_grid_size);

        s_grid_vertex_count = ((s_grid_size) * 4) + 6;
        Vector<VertexImmediate> verticies(s_grid_vertex_count);

        u32 index = 0;
        for (s32 x = -half_grid_size; x <= half_grid_size; x++) {
            if (x == 0) continue; // Skip center line

            f32 from_point = static_cast<f32>(x);
            Color m_color = (x % 10) == 0 ? special_grid_color : default_grid_color;
            verticies[index++] = { Vec3(from_point, 0, to_point), m_color };
            verticies[index++] = { Vec3(from_point, 0, -to_point), m_color };
        }
        for (s32 z = -half_grid_size; z <= half_grid_size; z++) {
            if (z == 0) continue; // Skip center line

            f32 from_point = static_cast<f32>(z);
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