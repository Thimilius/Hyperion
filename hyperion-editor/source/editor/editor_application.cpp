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
#include "hyperion/editor/editor_render_pipeline.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/editor_ui.hpp"

#define UI_TEST false

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  const String WORLD_PATH = "world.world";

  //--------------------------------------------------------------
  void EditorApplication::EnterRuntime() {
    if (Engine::GetEngineState() == EngineState::Editor) {
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
  void EditorApplication::CreateEntity(EntityPrimitive primitive) {
    EntityId entity = s_world->GetEntityManager()->CreateEntity(primitive);
    EditorSelection::Select(entity);
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
  void EditorApplication::OnSetup(ApplicationSettings &settings) {
    settings.window.menu.items = { {
      "File", { }, {
        { "Open World", [this](auto _) { OpenWorld(); }, { } },
        { "Save World", [this](auto _) { SaveWorld(); }, { } },
        { "Exit", [this](auto _) { Exit(); }, { } },
    } }, {
      "Help", { }, {
        { "About", [this](auto _) { OperatingSystem::MessageBox(MessageBoxType::Info, "About Hyperion", "Hyperion 0.1.0\n"); }, { } }
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
      EntityId parent = manager->CreateEntity(EntityPrimitive::Cube);
      EntityId child = manager->CreateEntity(EntityPrimitive::Sphere);
      manager->GetComponent<LocalTransformComponent>(child)->position = Vector3(2.0f, 0.0f, 0.0f);
      s_world->GetHierarchy()->SetParent(child, parent);

      String text = WorldSerializer::Serialize(s_world);
      FileSystem::WriteAllText(WORLD_PATH, text);
    }
    WorldManager::SetActiveWorld(s_world);

#if !UI_TEST
    EditorUI::Initialize();
#endif
    EditorCamera::Initialize();
  }

  //--------------------------------------------------------------
  void EditorApplication::OnUpdate(float32 delta_time) {
#if !UI_TEST
    EditorUI::Update();
#endif
    EditorCamera::Update(delta_time);

    if (Input::IsKeyHold(KeyCode::Control)) {
      if (Input::IsKeyDown(KeyCode::O)) {
        OpenWorld();
      }
      if (Input::IsKeyDown(KeyCode::S)) {
        SaveWorld();
      }
      if (Input::IsKeyDown(KeyCode::W)) {
        Exit();  
      }
      if (Input::IsKeyDown(KeyCode::D)) {
        DuplicateEntity();
      }
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F5)) {
      if (Engine::GetEngineState() == EngineState::Editor) {
        EnterRuntime();
      } else {
        ExitRuntime();
      }
    }

    Vector2 preview_size = EditorUI::GetPreviewRect().size;
    Display::SetPreviewSize(static_cast<uint32>(preview_size.x), static_cast<uint32>(preview_size.y));

#if UI_TEST
    static UI::UIImmediateTheme *red_theme = UI::UIImmediate::CreateTheme("Red");
    red_theme->panel_color = Color::Red();
    static UI::UIImmediateTheme *green_theme = UI::UIImmediate::CreateTheme("Green");
    green_theme->panel_color = Color::Green();
    static UI::UIImmediateTheme *yellow_theme = UI::UIImmediate::CreateTheme("Yellow");
    yellow_theme->panel_color = Color::Yellow();
    
    UI::UIImmediate::Begin();
    {
      UI::Size top_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::Pixels, 25.0f } };
      UI::UIImmediate::BeginPanel("Top Panel", top_size, UI::ChildLayout::Horizontal, false, red_theme);
      {
        /*
        UI::Size center_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
        UI::UIImmediate::Space(UI::SizeKind::Pixels, 25.0f);
        UI::UIImmediate::BeginPanel("Middle Panel", center_size, UI::ChildLayout::Horizontal, false, yellow_theme);
        {
          
        }
        UI::UIImmediate::EndPanel();
        UI::UIImmediate::Space(UI::SizeKind::Pixels, 25.0f);
        */

        UI::UIImmediate::Space(UI::SizeKind::Pixels, 25.0f);
        static String input = "Hello there";
        UI::UIImmediate::Input("Input", input, UI::TextAlignment::TopLeft, UI::FitType::Fill);
        UI::UIImmediate::Space(UI::SizeKind::Pixels, 25.0f);
      }
      UI::UIImmediate::EndPanel();

      UI::Size center_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
      UI::UIImmediate::BeginPanel("Center Panel", center_size, UI::ChildLayout::Horizontal, false, green_theme);
      {
        
      }
      UI::UIImmediate::EndPanel();
      
      UI::Size bottom_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::Pixels, 25.0f } };
      UI::UIImmediate::BeginPanel("Bottom Panel", bottom_size, UI::ChildLayout::Horizontal, false, yellow_theme);
      {
        
      }
      UI::UIImmediate::EndPanel();
    }
    UI::UIImmediate::End();
#endif
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
      }  
    }
  }
  
  //--------------------------------------------------------------
  void EditorApplication::SaveWorld() {
    String path = OperatingSystem::SaveFileDialog("Save World", ".world");
    if (!StringUtils::EndsWith(path, ".world")) {
      path += ".world";
    }
    String world_text = WorldSerializer::Serialize(s_world);
    FileSystem::WriteAllText(path, world_text);
  }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
  return new Hyperion::Editor::EditorApplication();
}
