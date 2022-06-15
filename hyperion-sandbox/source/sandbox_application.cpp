//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;
using namespace Hyperion::Rendering;

#if HYP_RELEASE || HYP_PROFILE
//#define HYP_STRESS_TEST
//#define HYP_STRESS_TEST_EXTREME
#endif

//-------------------- Definition Namespace --------------------
namespace Sandbox {

  //--------------------------------------------------------------
  void SandboxApplication::OnSetup(ApplicationSettings &settings) {
    settings.render.backend = RenderBackend::OpenGL;
    settings.render.threading_mode = RenderThreadingMode::MultiThreaded;
    settings.render.vsync_mode = VSyncMode::DontSync;

    settings.scripting.assembly_name = "Hyperion.Sandbox.dll";
  }

  World *g_world;
  EntityId g_camera;
  EntityId g_light;
  EntityId g_parent;
  EntityId g_child;

  CameraController *g_camera_controller;

  //--------------------------------------------------------------
  void SandboxApplication::OnInitialize() {
    // NOTE: This is only necessary as we are compiling with Hyperion with editor enabled.
    Engine::SetEngineState(EngineState::Runtime);
    
    g_world = WorldManager::CreateWorld();
    WorldManager::SetActiveWorld(g_world);

    EntityManager *manager = g_world->GetEntityManager();
    
    g_camera = manager->CreateEntity(EntityPrimitive::Camera);
    g_light = manager->CreateEntity(EntityPrimitive::DirectionalLight);

    g_camera_controller = new LookAroundCameraController(g_camera);
    g_camera_controller->Reset(manager);

    g_parent = manager->CreateEntity(EntityPrimitive::Sphere);
#ifdef HYP_STRESS_TEST
    float32 size = 100;
    for (float32 x = 0; x < size; x++) {
      for (float32 z = 0; z < size; z++) {
#ifdef HYP_STRESS_TEST_EXTREME
        Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
        material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
        EntityId entity = manager->CreateEntity(EntityPrimitive::Cube);
        manager->GetComponent<LocalTransformComponent>(entity)->position = Vector3(x * 2.0f, 0.0f, -z * 2.0f);
#ifdef HYP_STRESS_TEST_EXTREME
        manager->GetComponent<MeshComponent>(entity)->material = material;
#endif
        g_world->GetHierarchy()->SetParent(entity, g_parent);
        manager->RemoveComponent<Physics::BoxColliderComponent>(entity);
      }
    }

    for (uint32 i = 0; i < 128; i++) {
      float32 x = Random::Get() * size * 2.0f;
      float32 z = Random::Get() * size * 2.0f;

      EntityId point_light = manager->CreateEntity(EntityPrimitive::PointLight);
      manager->GetComponent<LocalTransformComponent>(point_light)->position = Vector3(x, 1.0f, -z);
      manager->GetComponent<PointLightComponent>(point_light)->color = Color(Random::Get(), Random::Get(), Random::Get(), 1.0f);
      manager->GetComponent<PointLightComponent>(point_light)->intensity = 5.0f;
    }
#else
    g_child = manager->CreateEntity(EntityPrimitive::Cube);
    manager->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
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
    EntityId quad = manager->CreateEntity(EntityPrimitive::Quad);
    manager->GetComponent<MeshComponent>(quad)->material = material->GetMetadata().handle;
    manager->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);
#endif
    
    UpdateTitle();
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnUpdate(float32 delta_time) {
    // NOTE: This is only necessary as we are compiling with Hyperion with editor enabled.
    Display::SetPreviewSize(Display::GetWidth(), Display::GetHeight());
    
    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F2)) {
      Rendering::RenderEngine::SetVSyncMode(
        Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? Rendering::VSyncMode::EveryVBlank : Rendering::VSyncMode::DontSync);
    }
    if (Input::IsKeyDown(KeyCode::F3)) {
      Rendering::RenderGizmos::SetShouldDrawGrid(!Rendering::RenderGizmos::GetShouldDrawGrid());
    }
    if (Input::IsKeyDown(KeyCode::F4)) {
      Rendering::RenderGizmos::SetShouldDrawAllBounds(!Rendering::RenderGizmos::GetShouldDrawAllBounds());
    }

    EntityManager *manager = g_world->GetEntityManager();
    
    g_camera_controller->Update(manager, delta_time);
    if (Input::IsKeyDown(KeyCode::R)) {
      g_camera_controller->Reset(manager);
    }

    manager->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);

#ifdef HYP_STRESS_TEST
    Quaternion rotation = Quaternion::FromEulerAngles(0.0f, Time::GetTime() * 25.0f, 0.0f);
    auto view = manager->GetView<LocalTransformComponent, MeshComponent>(ExcludeComponents<Physics::SphereColliderComponent>());
    for (EntityId entity : view) {
      LocalTransformComponent *transform = manager->GetComponent<LocalTransformComponent>(entity);
      transform->rotation = rotation;
#ifdef HYP_STRESS_TEST_EXTREME
      MeshComponent *mesh = manager->GetComponent<MeshComponent>(entity);
      mesh->material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
    }
#endif
    UpdateTitle();
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnTick() { }

  //--------------------------------------------------------------
  void SandboxApplication::UpdateTitle() {
    String format = "Sandbox - FPS: {} ({:.2f}ms) - VSync: {} - Draw calls: {}, Vertices: {}, Triangles: {} - Memory: {}";
    RenderStats render_stats = Rendering::RenderEngine::GetStats();
    String vsync = Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? "Off" : "On";
    uint64 memory = MemoryStats::GetGlobalMemory();
    String title = StringUtils::Format(format,
                                       Time::GetFPS(),
                                       Time::GetFrameTime(),
                                       vsync,
                                       render_stats.draw_calls,
                                       render_stats.vertex_count,
                                       render_stats.triangle_count,
                                       memory);

    GetMainWindow()->SetTitle(title);
  }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
  return new Sandbox::SandboxApplication();
}
