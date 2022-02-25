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
using namespace Hyperion;
using namespace Hyperion::Rendering;

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

  World *g_world;
  EntityId g_camera;
  EntityId g_light;
  EntityId g_parent;
  EntityId g_child;

  CameraController *g_camera_controller;

  //--------------------------------------------------------------
  void SandboxApplication::OnInitialize() {
    g_world = WorldManager::CreateWorld();
    WorldManager::SetActiveWorld(g_world);

    g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
    g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);

    g_camera_controller = new LookAroundCameraController(g_camera);
    g_camera_controller->Reset(g_world);
    LocalTransformComponent *camera_transform = g_world->GetComponent<LocalTransformComponent>(g_camera);

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
#endif

    UpdateTitle();
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnUpdate(float32 delta_time) {
    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
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
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnTick() {

  }

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