//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/core/engine.hpp>
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
#include <hyperion/ui/ui_factory.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_style.hpp"
#include "hyperion/editor/editor_ui.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  const String WORLD_PATH = "world.world";

  //--------------------------------------------------------------
  void EditorApplication::EnterRuntime() {
    if (Engine::GetEngineMode() == EngineMode::Editor || Engine::GetEngineMode() == EngineMode::EditorRuntimePaused) {
      Engine::SetEngineMode(EngineMode::EditorRuntimePlaying);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::PauseRuntime() {
    if (Engine::GetEngineMode() == EngineMode::EditorRuntimePlaying) {
      Engine::SetEngineMode(EngineMode::EditorRuntimePaused);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::ExitRuntime() {
    if (Engine::GetEngineMode() == EngineMode::EditorRuntimePlaying || Engine::GetEngineMode() == EngineMode::EditorRuntimePaused) {
      Engine::SetEngineMode(EngineMode::Editor);

      WorldManager::SetActiveWorld(nullptr);
      WorldManager::DestroyWorld(s_world);
      s_world = WorldSerializer::Deserialize(FileSystem::ReadAllText(WORLD_PATH));
      WorldManager::SetActiveWorld(s_world);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::OnSetup(ApplicationSettings &settings) {
    settings.window.menu.items.Add({ "File", { }, {
        { "New", [](auto _) { HYP_TRACE("NEW"); }, {}},
        { "Open", [](auto _) { HYP_TRACE("OPEN"); } , { } },
        { "Save", [](auto _) { HYP_TRACE("SAVE"); } , { } },
    } });
    settings.window.menu.items.Add({ "Edit", { }, { } });

    s_render_pipeline = new EditorRenderPipeline();
    settings.render.backend = Rendering::RenderBackend::OpenGL;
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = s_render_pipeline;
  }

  //--------------------------------------------------------------
  void EditorApplication::OnInitialize() {
    if (FileSystem::Exists(WORLD_PATH)) {
      String text = FileSystem::ReadAllText(WORLD_PATH);
      s_world = WorldSerializer::Deserialize(text);
    } else {
      s_world = WorldManager::CreateWorld();
      EntityId camera = s_world->CreateEntity(EntityPrimitive::Camera);
      s_world->GetComponent<LocalTransformComponent>(camera)->position = Vector3(0.0f, 1.0f, 5.0f);
      EntityId light = s_world->CreateEntity(EntityPrimitive::DirectionalLight);
      s_world->GetComponent<LocalTransformComponent>(light)->rotation = Quaternion::FromEulerAngles(-30.0f, 20.0f, 0.0f);
      EntityId parent = s_world->CreateEntity(EntityPrimitive::Cube);
      EntityId child = s_world->CreateEntity(EntityPrimitive::Sphere);
      s_world->GetComponent<LocalTransformComponent>(child)->position = Vector3(2.0f, 0.0f, 0.0f);
      s_world->GetHierarchy()->SetParent(child, parent);

      String text = WorldSerializer::Serialize(s_world);
      FileSystem::WriteAllText(WORLD_PATH, text);
    }
    WorldManager::SetActiveWorld(s_world);

    EditorUI::Initialize();
    EditorCamera::Initialize();
  }

  //--------------------------------------------------------------
  void EditorApplication::OnUpdate(float32 delta_time) {
    EditorUI::Update();
    EditorCamera::Update(delta_time);

    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F5)) {
      if (Engine::GetEngineMode() == EngineMode::Editor) {
        EnterRuntime();
      } else {
        ExitRuntime();
      }
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::OnTick() {

  }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
  return new Hyperion::Editor::EditorApplication();
}