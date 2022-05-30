//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/system/engine.hpp>
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
    settings.window.menu.items.Add({
      "File", { }, {
        { "New", [](auto _) { HYP_TRACE("NEW"); }, { } },
        { "Open", [](auto _) { HYP_TRACE("OPEN"); }, { } },
        { "Save", [](auto _) { HYP_TRACE("SAVE"); }, { } },
      }
    });
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

    //EditorUI::Initialize();
    EditorCamera::Initialize();

    UI::UIImmediateTheme *theme = UI::UIImmediate::GetDefaultTheme();
    theme->separator_color = EditorStyle::COLOR_HIGHLIGHT;
    theme->panel_color = EditorStyle::COLOR_NORMAL;
    theme->panel_color_hover = EditorStyle::COLOR_NORMAL;
    theme->panel_color_pressed = EditorStyle::COLOR_NORMAL;
    theme->button_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->button_color_hover = EditorStyle::COLOR_HIGHLIGHT;
    theme->button_color_pressed = EditorStyle::COLOR_HIGHLIGHT;
  }

  //--------------------------------------------------------------
  void EditorApplication::OnUpdate(float32 delta_time) {
    //EditorUI::Update();
    EditorCamera::Update(delta_time);

    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F5)) {
      if (Engine::GetEngineMode() == EngineMode::Editor) {
        EnterRuntime();
      } else {
        ExitRuntime();
      }
    }

    UI::UIImmediate::Begin();
    {
      UI::Size header_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::Pixels, 25.0f } };
      UI::UIImmediate::BeginPanel("Header Panel", header_panel_size);
      {
        if (UI::UIImmediate::Button("Left Aligned Button").clicked) {
          HYP_TRACE("CLICKED BUTTON");
        }

        UI::UIImmediate::FillSpace();
        
        if (UI::UIImmediate::Button("Right Aligned Button").clicked) {
          HYP_TRACE("CLICKED BUTTON");
        }

        UI::UIImmediate::BeginCenter();
        {
          if (UI::UIImmediate::Button("Center Aligned Button").clicked) {
            HYP_TRACE("CLICKED BUTTON");
          }
        }
        UI::UIImmediate::EndCenter();
      }
      UI::UIImmediate::EndPanel();

      UI::UIImmediate::Separator();
      
      UI::Size center_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
      UI::UIImmediate::BeginPanel("Center Panel", center_panel_size);
      {
        UI::Size left_panel_size[2] = { { UI::SizeKind::PercentOfParent, 0.3f }, { UI::SizeKind::AutoFill, 0.0f } };
        UI::UIImmediate::BeginPanel("Left Panel", left_panel_size, UI::ChildLayout::Vertical);
        {
          UI::Size upper_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
          UI::UIImmediate::BeginPanel("Upper Panel", upper_panel_size);
          {
          
          }
          UI::UIImmediate::EndPanel();

          UI::UIImmediate::Separator();
          
          UI::Size lower_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
          UI::UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
          
          }
          UI::UIImmediate::EndPanel();
        }
        UI::UIImmediate::EndPanel();

        UI::UIImmediate::Separator();
        
        UI::Size right_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
        UI::UIImmediate::BeginPanel("Right Panel", right_panel_size, UI::ChildLayout::Vertical);
        {
          UI::Size preview_container_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
          UI::UIImmediate::BeginPanel("Preview Container", preview_container_panel_size, UI::ChildLayout::Vertical);
          {
            UI::Size preview_header_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::Pixels, 25.0f } };
            UI::UIImmediate::BeginPanel("Preview Header", preview_header_panel_size);
            {
              UI::UIImmediate::FillSpace();

              String stats_format = "FPS: {} ({:.2f}ms)";
              String stats_text = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
              UI::UIImmediate::Text(stats_text);
            }
            UI::UIImmediate::EndPanel();

            UI::UIImmediate::Separator();
          
            UI::Size preview_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
            UI::UIImmediate::BeginPanel("Preview", preview_panel_size);
            {
              
            }
            UI::UIImmediate::EndPanel();
          }
          UI::UIImmediate::EndPanel();

          UI::UIImmediate::Separator();
          
          UI::Size lower_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::PercentOfParent, 0.3f } };
          UI::UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
            UI::Size left_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
            UI::UIImmediate::BeginPanel("Left Panel", left_panel_size);
            {
          
            }
            UI::UIImmediate::EndPanel();

            UI::UIImmediate::Separator();
          
            UI::Size right_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::AutoFill, 0.0f } };
            UI::UIImmediate::BeginPanel("Right Panel", right_panel_size);
            {
              if (UI::UIImmediate::Button("Center Aligned Button").clicked) {
                HYP_TRACE("CLICKED BUTTON");
              }
            }
            UI::UIImmediate::EndPanel();
          }
          UI::UIImmediate::EndPanel();
        }
        UI::UIImmediate::EndPanel();
      }
      UI::UIImmediate::EndPanel();

      UI::UIImmediate::Separator();
      
      UI::Size footer_panel_size[2] = { { UI::SizeKind::AutoFill, 0.0f }, { UI::SizeKind::Pixels, 25.0f } };
      UI::UIImmediate::BeginPanel("Footer Panel", footer_panel_size);
      {
        
      }
      UI::UIImmediate::EndPanel();
    }
    UI::UIImmediate::End();
  }

  //--------------------------------------------------------------
  void EditorApplication::OnTick() { }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
  return new Hyperion::Editor::EditorApplication();
}
