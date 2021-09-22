//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/assets/utilities/mesh_generator.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>

//---------------------- Project Includes ----------------------
#include "sandbox/camera_controller.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion;
using namespace Hyperion::Rendering;

//#define HYP_STRESS_TEST

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void SandboxApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.backend = Rendering::RenderBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::SingleThreaded;
        settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
    }

    World *g_world;
    EntityId g_camera;
    EntityId g_light;
    EntityId g_parent;
    EntityId g_child;

    CameraController *g_camera_controller;

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        UpdateTitle();

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
                Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
                material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));

                EntityId entity = g_world->CreateEntity(EntityPrimitive::Cube);
                g_world->GetComponent<LocalTransformComponent>(entity)->position = Vector3(x * 2.0f, 0.0f, -z * 2.0f);
                g_world->GetComponent<RenderMeshComponent>(entity)->material = material;
                g_world->GetHierarchy()->SetParent(entity, g_parent);
                g_world->RemoveComponent<Physics::BoxColliderComponent>(entity);
            }
        }
#else
        g_child = g_world->CreateEntity(EntityPrimitive::Cube);
        g_world->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
        g_world->GetHierarchy()->SetParent(g_child, g_parent);
#endif
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

        g_camera_controller->Update(g_world, delta_time);
        if (Input::IsKeyDown(KeyCode::R)) {
            g_camera_controller->Reset(g_world);
        }

        g_world->GetComponent<LocalTransformComponent>(g_light)->rotation = Quaternion::FromEulerAngles(-Time::GetTime() * 25.0f, 0.0f, 0.0f);

        Quaternion rotation = Quaternion::FromEulerAngles(0.0f, Time::GetTime() * 25.0f, 0.0f);

        LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(g_parent);
        transform->rotation = rotation;

#ifdef HYP_STRESS_TEST
        auto view = g_world->GetView<LocalTransformComponent, RenderMeshComponent>();
        for (EntityId entity : view) {
            LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(entity);
            transform->rotation = rotation;
            RenderMeshComponent *render_mesh = g_world->GetComponent<RenderMeshComponent>(entity);
            render_mesh->material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
        }
#endif
        UpdateTitle();
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