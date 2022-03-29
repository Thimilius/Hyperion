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

  CameraController *g_camera_controller;

  //--------------------------------------------------------------
  void SetMaterialByIndex(World *world, EntityId entity, Material *material, uint32 index) {
    world->GetComponent<MeshComponent>(world->GetHierarchy()->GetChild(entity, index))->material = material;
  }

  //--------------------------------------------------------------
  void MedhexApplication::OnInitialize() {
    RenderGizmos::SetShouldDrawGrid(false);

    g_world = WorldManager::CreateWorld();
    WorldManager::SetActiveWorld(g_world);

    g_camera = g_world->CreateEntity(EntityPrimitive::Camera);
    g_light = g_world->CreateEntity(EntityPrimitive::DirectionalLight);
    g_world->GetComponent<LocalTransformComponent>(g_light)->rotation = Quaternion::FromEulerAngles(-45.0f, 45.0f, 0.0f);

    Material *material_ground = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
    material_ground->SetColor("m_color", Color(0.8862745f, 0.5137255f, 0.3411765f, 1.0f));
    Material *material_grass = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
    material_grass->SetColor("m_color", Color(0.2065237f, 0.8584906f, 0.6385639f, 1.0f));

    Mesh *mesh = MeshLoader::Load("data/models/medieval/grass.obj").Unwrap();
    EntityId hex = g_world->CreateMultiMeshEntity(mesh);
    SetMaterialByIndex(g_world, hex, material_ground, 0);
    SetMaterialByIndex(g_world, hex, material_grass, 1);

    g_camera_controller = new LookAroundCameraController(g_camera);
    g_camera_controller->Reset(g_world);

    UpdateTitle();
  }

  //--------------------------------------------------------------
  void MedhexApplication::OnUpdate(float32 delta_time) {
    UpdateInput(delta_time);
    UpdateTitle();
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