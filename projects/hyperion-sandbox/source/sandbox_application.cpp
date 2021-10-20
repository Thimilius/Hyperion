//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

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
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>
#include <hyperion/ui/ui_button.hpp>
#include <hyperion/ui/ui_label.hpp>

//---------------------- Project Includes ----------------------
#include "sandbox/camera_controller.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion;
using namespace Hyperion::Rendering;
using namespace Hyperion::UI;

#if HYP_RELEASE || HYP_PROFILE
    #define HYP_STRESS_TEST
    //#define HYP_STRESS_TEST_EXTREME
#endif

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void SandboxApplication::OnSetup(ApplicationSettings &settings) {
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
    ForwardRenderPipeline *g_render_pipeline;

    CameraController *g_camera_controller;

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        UpdateTitle();

        g_render_pipeline = static_cast<ForwardRenderPipeline *>(RenderEngine::GetPipeline());
        g_render_pipeline->SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - UI_HEADER_SIZE);

        g_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(g_world);

        g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
        g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);

        g_camera_controller = new LookAroundCameraController(g_camera);
        g_camera_controller->Reset(g_world);
        LocalTransformComponent *camera_transform = g_world->GetComponent<LocalTransformComponent>(g_camera);
#ifdef HYP_STRESS_TEST
        camera_transform->position = Vector3(0.0f, 15.0f, 0.0f);
        camera_transform->rotation = Quaternion::FromEulerAngles(-45.0f, -45.0f, 0.0f);
#else
        camera_transform->position = Vector3(0.0f, 1.5f, 3.0f);
        camera_transform->rotation = Quaternion::FromEulerAngles(-25.0f, 0.0f, 0.0f);
#endif

        g_parent = g_world->CreateEntity(EntityPrimitive::Sphere);
#ifdef HYP_STRESS_TEST
        float32 size = 100;
        for (float32 x = 0; x < size; x++) {
            for (float32 z = 0; z < size; z++) {
#ifdef HYP_STRESS_TEST_EXTREME
                Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
                material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
                EntityId entity = g_world->CreateEntity(EntityPrimitive::Cube);
                g_world->GetComponent<LocalTransformComponent>(entity)->position = Vector3(x * 2.0f, 0.0f, -z * 2.0f);
#ifdef HYP_STRESS_TEST_EXTREME
                g_world->GetComponent<MeshComponent>(entity)->material = material;
#endif
                g_world->GetHierarchy()->SetParent(entity, g_parent);
                g_world->RemoveComponent<Physics::BoxColliderComponent>(entity);
            }
        }

        for (uint32 i = 0; i < 128; i++) {
            float32 x = Random::Get() * size * 2.0f;
            float32 z = Random::Get() * size * 2.0f;

            EntityId point_light = g_world->CreateEntity(EntityPrimitive::PointLight);
            g_world->GetComponent<LocalTransformComponent>(point_light)->position = Vector3(x, 1.0f, -z);
            g_world->GetComponent<PointLightComponent>(point_light)->color = Color(Random::Get(), Random::Get(), Random::Get(), 1.0f);
            g_world->GetComponent<PointLightComponent>(point_light)->intensity = 5.0f;
        }
#else
        g_child = g_world->CreateEntity(EntityPrimitive::Cube);
        g_world->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
        g_world->GetHierarchy()->SetParent(g_child, g_parent);

        Image *image = ImageLoader::Load("icon/icon.png").Unwrap();
        Texture2DParameters parameters;
        parameters.format = TextureFormat::RGBA32;
        parameters.width = image->GetWidth();
        parameters.height = image->GetHeight();
        Texture2D *texture = AssetManager::CreateTexture2D(parameters, image->GetPixels());
        delete image;
        Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Unlit));
        material->SetTexture("m_texture", texture);
        EntityId quad = g_world->CreateEntity(EntityPrimitive::Quad);
        g_world->GetComponent<MeshComponent>(quad)->material = material;
        g_world->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);
#endif
        UIElement *root_element = new UIElement();
        root_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
        root_element->GetStyle().SetVisibility(UIVisibility::Visible);

        g_render_ui_element = new UIElement();
        g_render_ui_element->SetAnchorPreset(UIAnchorPreset::StretchAll);
        g_render_ui_element->SetOffsetMax(Vector2(0.0f, UI_HEADER_SIZE));
        g_render_ui_element->GetRenderer().texture = g_render_pipeline->GetTargetRenderTexture();
        g_render_ui_element->GetRenderer().render_texture_attachment_index = 1;
        g_render_ui_element->GetRenderer().enable_blending = false;
        root_element->GetHierarchy().AddChild(g_render_ui_element);

        g_header_ui_element = new UIElement();
        g_header_ui_element->SetSize(Vector2(0.0f, UI_HEADER_SIZE));
        g_header_ui_element->SetAnchorPreset(UIAnchorPreset::TopStretchHorizontal);
        g_header_ui_element->GetStyle().SetColor(Color::Grey());
        root_element->GetHierarchy().AddChild(g_header_ui_element);

        UILabel *label = new UILabel();
        label->SetFont(FontLoader::LoadFont("data/fonts/consola.ttf", 24, FontCharacterSet::LatinSupplement));
        label->SetText("Hello there!");
        g_header_ui_element->GetHierarchy().AddChild(label);

        EntityId ui = g_world->CreateEntity();
        UIViewComponent *ui_view = g_world->AddComponent<UIViewComponent>(ui);
        ui_view->root_element = root_element;
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnUpdate(float32 delta_time) {
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

#ifdef HYP_STRESS_TEST
        Quaternion rotation = Quaternion::FromEulerAngles(0.0f, Time::GetTime() * 25.0f, 0.0f);
        auto view = g_world->GetView<LocalTransformComponent, MeshComponent>(ExcludeComponents<Physics::SphereColliderComponent>());
        for (EntityId entity : view) {
            LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(entity);
            transform->rotation = rotation;
#ifdef HYP_STRESS_TEST_EXTREME
            MeshComponent *mesh = g_world->GetComponent<MeshComponent>(entity);
            mesh->material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
        }
#endif
        UpdateTitle();

        g_world->GetComponent<CameraComponent>(g_camera)->viewport_clipping.height = (Display::GetHeight() - UI_HEADER_SIZE) / static_cast<float32>(Display::GetHeight());
        if (Display::HasChangedSize()) {
            g_render_pipeline->SetRenderTargetSize(Display::GetWidth(), Display::GetHeight() - UI_HEADER_SIZE);
        }
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnTick() {

    }

    //--------------------------------------------------------------
    void SandboxApplication::UpdateTitle() {
        String format = "Hyperion - FPS: {} ({:.2f}ms) - VSync: {} - Draw calls: {}, Vertices: {}, Triangles: {}";
        RenderStats render_stats = Rendering::RenderEngine::GetStats();
        String vsync = Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? "Off" : "On";
        String title = StringUtils::Format(format, Time::GetFPS(), Time::GetFrameTime(), vsync, render_stats.draw_calls, render_stats.vertex_count, render_stats.triangle_count);
        GetWindow()->SetTitle(title);
    }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
    return new Sandbox::SandboxApplication();
}