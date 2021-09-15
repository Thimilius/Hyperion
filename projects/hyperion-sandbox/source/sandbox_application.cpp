//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/assets/utilities/mesh_generator.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/system/transform_system.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;

//#define HYP_STRESS_TEST

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void SandboxApplication::OnSetup(ApplicationSettings &settings) {
        settings.render.graphics_backend = Graphics::GraphicsBackend::OpenGL;
        settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    }

    World *g_world;
    EntityId g_camera;
    EntityId g_parent;
    EntityId g_child;
    Material *g_material;

    //--------------------------------------------------------------
    void SandboxApplication::OnInitialize() {
        UpdateTitle();

        g_world = WorldManager::CreateWorld();

        WorldManager::SetActiveWorld(g_world);
        g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
        CameraComponent *camera = g_world->GetComponent<CameraComponent>(g_camera);
        camera->background_color = Color(0.0f, 1.0f, 1.0f, 1.0f);

        LocalTransformComponent *camera_transform = g_world->GetComponent<LocalTransformComponent>(g_camera);
#ifdef HYP_STRESS_TEST
        camera_transform->position = Vector3(0.0f, 15.0f, 0.0f);
        camera_transform->rotation = Quaternion::FromEulerAngles(-45.0f, -45.0f, 0.0f);
#else
        camera_transform->position = Vector3(0.0f, 1.5f, 3.0f);
        camera_transform->rotation = Quaternion::FromEulerAngles(-25.0f, 0.0f, 0.0f);
#endif

        Shader *shader = AssetManager::CreateShader(FileSystem::ReadAllText("data/shaders/standard.shader"));
        g_material = AssetManager::CreateMaterial(shader);

        Mesh *mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube); MeshLoader::Load("data/models/monkey.obj").Unwrap();

#ifdef HYP_STRESS_TEST
        float32 size = 100;
        for (float32 x = 0; x < size; x++) {
            for (float32 z = 0; z < size; z++) {
                Material *material = AssetManager::CreateMaterial(shader);
                material->SetColor("u_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));

                EntityId entity = g_world->CreateEntity();
                LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(entity);
                transform->position = Vector3(x * 2.0f, 0.0f, -z * 2.0f);
                RenderMeshComponent *render_mesh = g_world->AddComponent<RenderMeshComponent>(entity);
                render_mesh->material = material;
                render_mesh->mesh = mesh;
            }
        }
#else
        g_parent = g_world->CreateEntity();
        g_child = g_world->CreateEntity();

        g_world->GetHierarchy().SetParent(g_child, g_parent);

        RenderMeshComponent *render_mesh = g_world->AddComponent<RenderMeshComponent>(g_parent);
        render_mesh->material = g_material;
        render_mesh->mesh = mesh;
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

        Quaternion rotation = Quaternion::FromEulerAngles(0.0f, Time::GetTime() * 25.0f, 0.0f);
#ifdef HYP_STRESS_TEST
        auto view = g_world->GetView<LocalTransformComponent, RenderMeshComponent>();
        for (EntityId entity : view) {
            LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(entity);
            transform->rotation = rotation;
            RenderMeshComponent *render_mesh = g_world->GetComponent<RenderMeshComponent>(entity);
            render_mesh->material->SetColor("u_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
        }
#else
        LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(g_parent);
        transform->rotation = rotation;
#endif
    }

    //--------------------------------------------------------------
    void SandboxApplication::OnTick() {
        UpdateTitle();
    }

    //--------------------------------------------------------------
    void SandboxApplication::UpdateTitle() {
        String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms)", Time::GetFPS(), Time::GetFrameTime());
        GetWindow()->SetTitle(title);
    }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
    return new Sandbox::SandboxApplication();
}