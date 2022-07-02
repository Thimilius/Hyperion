//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/random.hpp>
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/core/system/operating_system.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/ui/immediate/ui_immediate.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_input.hpp"
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/editor_ui.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  const String WORLD_PATH = "world.world";

  //--------------------------------------------------------------
  void EditorApplication::EnterRuntime() {
    if (Engine::GetEngineState() == EngineState::Editor) {
      if (s_editor_logger.ShouldClearOnPlay()) {
        s_editor_logger.Clear();
      }
      
      EditorSelection::Deselect();
      
      World *copy = WorldManager::CopyWorld(s_world);
      WorldManager::SetActiveWorld(copy);
      s_old_world = s_world;
      s_world = copy;

      Engine::SetEngineState(EngineState::EditorRuntimePlaying);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::PauseRuntime() {
    if (Engine::GetEngineState() == EngineState::EditorRuntimePlaying) {
      Engine::SetEngineState(EngineState::EditorRuntimePaused);
    } else if (Engine::GetEngineState() == EngineState::EditorRuntimePaused) {
      Engine::SetEngineState(EngineState::EditorRuntimePlaying);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::ExitRuntime() {
    if (Engine::GetEngineState() == EngineState::EditorRuntimePlaying || Engine::GetEngineState() == EngineState::EditorRuntimePaused) {
      EditorSelection::Deselect();
      
      Engine::SetEngineState(EngineState::Editor);

      World *world = s_world;
      WorldManager::SetActiveWorld(s_old_world);
      WorldManager::DestroyWorld(world);
      s_world = s_old_world;
      s_old_world = nullptr;
    }
  }

  //--------------------------------------------------------------
  EntityId EditorApplication::CreateEntity(EntityPrimitive primitive) {
    EntityId entity = s_world->GetEntityManager()->CreateEntity(primitive);
    EditorSelection::Select(entity);
    return entity;
  }

  //--------------------------------------------------------------
  void EditorApplication::DuplicateEntity() {
    if (EditorSelection::HasSelection()) {
      EntityId entity = EditorSelection::GetSelection();
      EntityId duplicate = s_world->GetEntityManager()->InstantiateEntity(entity);
      EditorSelection::Select(duplicate);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::DestroyEntity() {
    if (EditorSelection::HasSelection()) {
      EntityId entity = EditorSelection::GetSelection();
      s_world->GetEntityManager()->DestroyEntity(entity);
      EditorSelection::Deselect();
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::NewWorld() {
    if (Engine::GetEngineState() == EngineState::Editor) {
      World *world = WorldManager::CreateWorld();
      WorldManager::SetActiveWorld(world);
      WorldManager::DestroyWorld(s_world);
      s_world = world;
      
      EditorSelection::Deselect();
    }
  }
  
  //--------------------------------------------------------------
  void EditorApplication::OpenWorld() {
    if (Engine::GetEngineState() == EngineState::Editor) {
      String path = OperatingSystem::OpenFileDialog("Open World", ".world");
      if (FileSystem::Exists(path) && StringUtils::EndsWith(path, ".world")) {
        String world_text = FileSystem::ReadAllText(path);
        World *world = WorldSerializer::Deserialize(world_text);
        WorldManager::SetActiveWorld(world);
        WorldManager::DestroyWorld(s_world);
        s_world = world;

        EditorSelection::Deselect();

        HYP_LOG_INFO("Editor", "Opened world at path: {}", path);
      }  
    }
  }
  
  //--------------------------------------------------------------
  void EditorApplication::SaveWorld() {
    String path = OperatingSystem::SaveFileDialog("Save World", ".world");
    if (!path.empty()) {
      if (!StringUtils::EndsWith(path, ".world")) {
        path += ".world";
      }
      String world_text = WorldSerializer::Serialize(s_world);
      FileSystem::WriteAllText(path, world_text);
      HYP_LOG_INFO("Editor", "Saved world at path: {}", path);
    }
  }

  //--------------------------------------------------------------
  void EditorApplication::OnSetup(ApplicationSettings &settings) {
    settings.window.menu.items = { {
      "File", "", { }, { }, {
        { "New World", "Ctrl+N", [this]() { NewWorld(); }, { }, { } },
        { "Open World", "Ctrl+O", [this]() { OpenWorld(); }, { }, { } },
        { "Save World", "Ctrl+S", [this]() { SaveWorld(); }, { }, { } },
        MenuItem::Separator(),
        { "Exit", "Ctrl+W", [this]() { Exit(); }, { }, { } },
    } }, {
      "Help", "", { }, { }, {
        { "About", "", [this]() { OperatingSystem::MessageBox(MessageBoxType::Info, "About Hyperion", "Hyperion 0.1.0\n"); }, { }, { } }
    } } };

    s_render_pipeline = new EditorRenderPipeline();
    settings.render.backend = Rendering::RenderBackend::OpenGL;
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.vsync_mode = Rendering::VSyncMode::DontSync;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = s_render_pipeline;

    settings.scripting.assembly_name = "Hyperion.Editor.dll";
  }
  
  //--------------------------------------------------------------
  void EditorApplication::OnInitialize() {
    Log::RegisterLogger(&s_editor_logger);
    
    if (FileSystem::Exists(WORLD_PATH)) {
      String text = FileSystem::ReadAllText(WORLD_PATH);
      s_world = WorldSerializer::Deserialize(text);
    } else {
      s_world = WorldManager::CreateWorld();
      EntityManager *manager = s_world->GetEntityManager();
      
      EntityId camera = manager->CreateEntity(EntityPrimitive::Camera);
      manager->GetComponent<LocalTransformComponent>(camera)->position = Vector3(0.0f, 1.0f, 5.0f);
      EntityId light = manager->CreateEntity(EntityPrimitive::DirectionalLight);
      manager->GetComponent<LocalTransformComponent>(light)->rotation = Quaternion::FromEulerAngles(-30.0f, 20.0f, 0.0f);
      EntityId ground = manager->CreateEntity(EntityPrimitive::Plane);
      manager->GetComponent<LocalTransformComponent>(ground)->position = Vector3(0.0f, -0.5f, 0.0f);
      EntityId parent = manager->CreateEntity(EntityPrimitive::Cube);
      EntityId child = manager->CreateEntity(EntityPrimitive::Cube);
      manager->GetComponent<LocalTransformComponent>(child)->position = Vector3(2.0f, 0.0f, 0.0f);
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
    EditorInput::Update();
    EditorUI::Draw();
    EditorCamera::Update(delta_time);

    Vector2 preview_size = EditorUI::GetPreviewRect().size;
    Display::SetPreviewSize(static_cast<uint32>(preview_size.x), static_cast<uint32>(preview_size.y));
  }

  void EditorApplication::OnShutdown() {
    Log::UnregisterLogger(&s_editor_logger);
  }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
  return new Hyperion::Editor::EditorApplication();
}
