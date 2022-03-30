//--------------------- Definition Include ---------------------
#include "medhex/medhex_application.hpp"

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

//-------------------- Definition Namespace --------------------
namespace Medhex {

  struct HexComponent : public IComponent {
    int32 q = 0;
    int32 r = 0;
    int32 s = 0;

    float32 seed = 0.0f;
  };

  //--------------------------------------------------------------
  void MedhexApplication::OnSetup(ApplicationSettings &settings) {
    settings.render.backend = RenderBackend::OpenGL;
    settings.render.threading_mode = RenderThreadingMode::MultiThreaded;
    settings.render.vsync_mode = VSyncMode::DontSync;
  }

  World *g_world;
  EntityId g_camera;
  EntityId g_light;
  EntityId g_parent;
  EntityId g_child;

  Mesh *g_mesh_hex;
  Material *g_material_ground;
  Material *g_material_grass;

  CameraController *g_camera_controller;

  //--------------------------------------------------------------
  void SetMaterialByIndex(EntityId entity, Material *material, uint32 index) {
    g_world->GetComponent<MeshComponent>(g_world->GetHierarchy()->GetChild(entity, index))->material = material;
  }

  //--------------------------------------------------------------
  EntityId CreateHex(int32 q, int32 r) {
    EntityId entity = g_world->CreateMultiMeshEntity(g_mesh_hex);
    SetMaterialByIndex(entity, g_material_ground, 0);
    SetMaterialByIndex(entity, g_material_grass, 1);

    HexComponent *hex = g_world->AddComponent<HexComponent>(entity);
    hex->q = q;
    hex->r = r;
    hex->s = -q - r;

    LocalTransformComponent *transform = g_world->GetComponent<LocalTransformComponent>(entity);

    float32 size = 0.5773503f;
    float32 width = Math::Sqrt(3.0f) * size;
    float32 height = 2.0f * size;

    float32 horizontal = width;
    float32 vertical = height * 3.0f / 4.0f;

    transform->position = Vector3(horizontal * (q + r / 2.0f), 0.0f, -vertical * r);

    return entity;
  }

  //--------------------------------------------------------------
  void MedhexApplication::RegisterTypes() {
    MetaRegistry::Reflect<HexComponent>("HexComponent")
      .Base<IComponent>();
  }

  //--------------------------------------------------------------
  void MedhexApplication::OnInitialize() {
    RenderGizmos::SetShouldDrawGrid(false);

    g_mesh_hex = MeshLoader::Load("data/models/medieval/grass.obj").Unwrap();
    g_material_ground = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
    g_material_ground->SetColor("m_color", Color(0.8862745f, 0.5137255f, 0.3411765f, 1.0f));
    g_material_grass = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
    g_material_grass->SetColor("m_color", Color(0.2065237f, 0.8584906f, 0.6385639f, 1.0f));

    g_world = WorldManager::CreateWorld();
    WorldManager::SetActiveWorld(g_world);

    g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
    g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);
    g_world->GetComponent<LocalTransformComponent>(g_light)->rotation = Quaternion::FromEulerAngles(-45.0f, 45.0f, 0.0f);
    g_camera_controller = new LookAroundCameraController(g_camera);
    g_camera_controller->Reset(g_world);

    int32 map_size = 10;
    for (int32 r = 0; r < map_size; r++) {
      int32 r_offset = r >> 1;
      for (int32 q = 0 - r_offset; q < map_size - r_offset; q++) {
        CreateHex(q, r);
      }
    }

    for (EntityId hex : g_world->GetView<HexComponent>()) {
      g_world->GetComponent<HexComponent>(hex)->seed = Random::Get() * Math::PI * 2.0f;
    }

    UpdateTitle();
  }

  //--------------------------------------------------------------
  void MedhexApplication::OnUpdate(float32 delta_time) {
    UpdateInput(delta_time);
    UpdateTitle();

    for (EntityId hex : g_world->GetView<HexComponent>()) {
      g_world->GetComponent<LocalTransformComponent>(hex)->position.y = 0.25f * Math::Sin(Time::GetTime() + g_world->GetComponent<HexComponent>(hex)->seed);
    }
  }

  //--------------------------------------------------------------
  void MedhexApplication::UpdateInput(float32 delta_time) {
    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F2)) {
      RenderEngine::SetVSyncMode(
        RenderEngine::GetVSyncMode() == VSyncMode::DontSync ?
        VSyncMode::EveryVBlank :
        VSyncMode::DontSync);
    }
    if (Input::IsKeyDown(KeyCode::F3)) {
      RenderGizmos::SetShouldDrawGrid(!RenderGizmos::GetShouldDrawGrid());
    }
    if (Input::IsKeyDown(KeyCode::F4)) {
      RenderGizmos::SetShouldDrawAllBounds(!RenderGizmos::GetShouldDrawAllBounds());
    }

    g_camera_controller->Update(g_world, delta_time);
    if (Input::IsKeyDown(KeyCode::R)) {
      g_camera_controller->Reset(g_world);
    }
  }

  //--------------------------------------------------------------
  void MedhexApplication::UpdateTitle() {
    String format = "Medhex - FPS: {} ({:.2f}ms) - VSync: {} - Draw calls: {}, Vertices: {}, Triangles: {}";
    RenderStats render_stats = RenderEngine::GetStats();
    String vsync = RenderEngine::GetVSyncMode() == VSyncMode::DontSync ? "Off" : "On";
    String title = StringUtils::Format(format,
      Time::GetFPS(),
      Time::GetFrameTime(),
      vsync,
      render_stats.draw_calls,
      render_stats.vertex_count,
      render_stats.triangle_count
    );

    GetMainWindow()->SetTitle(title);
  }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
  return new Medhex::MedhexApplication();
}