//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/assets/utilities/mesh_generator.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>
#include <hyperion/ui/ui_button.hpp>
#include <hyperion/ui/ui_label.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.backend = Rendering::RenderBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
        settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
    }

    const uint32 UI_HEADER_SIZE = 25;

    World *g_world;
    EntityId g_camera;
    EntityId g_light;
    EntityId g_parent;
    EntityId g_child;
    UIElement *g_header_ui_element;
    UIElement *g_render_ui_element;
    UIButton *g_child_ui_element;
    UILabel *g_label;
    UILabel *g_label_shadow;

    CameraController *g_camera_controller;

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        RenderEngine::GetPipeline()->SetShouldBlitToScreen(false);
        RenderEngine::GetPipeline()->SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - UI_HEADER_SIZE);

        g_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(g_world);

        g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
        g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);

        g_camera_controller = new LookAroundCameraController(g_camera);
        g_camera_controller->Reset(g_world);
        LocalTransformComponent *camera_transform = g_world->GetComponent<LocalTransformComponent>(g_camera);

        g_parent = g_world->CreateEntity(EntityPrimitive::Sphere);

        g_child = g_world->CreateEntity(EntityPrimitive::Cube);
        g_world->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
        g_world->GetHierarchy()->SetParent(g_child, g_parent);

        std::unique_ptr<Image> image;
        image.reset(ImageLoader::Load("icon/icon.png").Unwrap());
        Texture2DParameters parameters;
        parameters.format = TextureFormat::RGBA32;
        parameters.width = image->GetWidth();
        parameters.height = image->GetHeight();
        Texture2D *texture = AssetManager::CreateTexture2D(parameters, image->GetPixels());
        Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Unlit));
        material->SetTexture("m_texture", texture);
        EntityId quad = g_world->CreateEntity(EntityPrimitive::Quad);
        g_world->GetComponent<MeshComponent>(quad)->material = material;
        g_world->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);

        UIElement *root_element = new UIElement();
        root_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
        root_element->GetStyle().SetVisibility(UIVisibility::Visible);

        g_render_ui_element = new UIElement();
        g_render_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
        g_render_ui_element->SetOffsetMax(Vector2(0.0f, UI_HEADER_SIZE));
        g_render_ui_element->GetRenderer().texture = RenderEngine::GetPipeline()->GetTargetRenderTexture();
        g_render_ui_element->GetRenderer().render_texture_attachment_index = 1;
        g_render_ui_element->GetRenderer().enable_blending = false;
        root_element->GetHierarchy().AddChild(g_render_ui_element);

        g_header_ui_element = new UIElement();
        g_header_ui_element->SetSize(Vector2(0.0f, UI_HEADER_SIZE));
        g_header_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
        g_header_ui_element->GetStyle().SetColor(Color::Grey());
        root_element->GetHierarchy().AddChild(g_header_ui_element);

        Font *consola_font = FontLoader::LoadFont("data/fonts/consola.ttf", 12, FontCharacterSet::LatinSupplement);

        g_label_shadow = new UILabel();
        g_label_shadow->SetFont(consola_font);
        g_label_shadow->SetAlignment(UITextAlignment::MiddleLeft);
        g_label_shadow->SetAnchorPreset(UIAnchorPreset::StretchAll);
        g_label_shadow->SetOffsetMin(Vector2(5.0f, 0.0f));
        g_label_shadow->GetStyle().SetColor(Color::Black());
        g_label_shadow->SetPosition(Vector2(1.0f, -1.0f));
        g_header_ui_element->GetHierarchy().AddChild(g_label_shadow);

        g_label = new UILabel();
        g_label->SetFont(consola_font);
        g_label->SetAlignment(UITextAlignment::MiddleLeft);
        g_label->SetAnchorPreset(UIAnchorPreset::StretchAll);
        g_label->SetOffsetMin(Vector2(5.0f, 0.0f));
        g_header_ui_element->GetHierarchy().AddChild(g_label);

        EntityId ui = g_world->CreateEntity();
        UIViewComponent *ui_view = g_world->AddComponent<UIViewComponent>(ui);
        ui_view->scaling_mode = UIScalingMode::ConstantPixelSize;
        ui_view->root_element = root_element;

        UpdateStats();
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
        if (Input::IsKeyDown(KeyCode::F2)) {
            Rendering::RenderEngine::SetVSyncMode(
                Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ?
                Rendering::VSyncMode::EveryVBlank :
                Rendering::VSyncMode::DontSync);
        }
        if (Input::IsKeyDown(KeyCode::F3)) {
            Rendering::RenderGizmos::SetShouldDrawGrid(!Rendering::RenderGizmos::GetShouldDrawGrid());
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            Rendering::RenderGizmos::SetShouldDrawAllBounds(!Rendering::RenderGizmos::GetShouldDrawAllBounds());
        }

        g_camera_controller->Update(g_world, delta_time);
        if (Input::IsKeyDown(KeyCode::R)) {
            g_camera_controller->Reset(g_world);
        }

        g_world->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);

        UpdateStats();

        g_world->GetComponent<CameraComponent>(g_camera)->viewport_clipping.height = (Display::GetHeight() - UI_HEADER_SIZE) / static_cast<float32>(Display::GetHeight());
        if (Display::HasChangedSize()) {
            RenderEngine::GetPipeline()->SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - UI_HEADER_SIZE);
        }
    }

    //--------------------------------------------------------------
    void EditorApplication::OnTick() {

    }

    //--------------------------------------------------------------
    void EditorApplication::UpdateStats() {
        String format = "FPS: {} ({:.2f}ms) - VSync: {} - Draw calls: {}, Vertices: {}, Triangles: {} - Memory: {}";
        RenderStats render_stats = Rendering::RenderEngine::GetStats();
        String vsync = Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? "Off" : "On";
        uint64 memory = MemoryStats::GetGlobalMemory();
        String title = StringUtils::Format(format, Time::GetFPS(), Time::GetFrameTime(), vsync, render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count, memory);
        g_label->SetText(title);
        g_label_shadow->SetText(title);
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    return new Hyperion::Editor::EditorApplication();
}