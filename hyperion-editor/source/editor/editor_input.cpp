//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_input.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/system/engine.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/ui/immediate/ui_immediate.hpp>
#include <hyperion/ui/immediate/ui_immediate_gizmos.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_ui.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  //--------------------------------------------------------------
  void EditorInput::Update() {
    // We do not want to check any keyboard shortcuts when we have a focused element.
    if (UI::UIImmediate::HasFocusedElement()) {
      return;
    }

    EditorApplication *application = static_cast<EditorApplication *>(EditorApplication::GetInstance());
    Window *main_window = application->GetMainWindow();
    
    if (Input::IsKeyHold(KeyCode::Control)) {
      if (Input::IsKeyDown(KeyCode::N)) {
        EditorApplication::NewWorld();
      }
      if (Input::IsKeyDown(KeyCode::O)) {
        EditorApplication::OpenWorld();
      }
      if (Input::IsKeyDown(KeyCode::S)) {
        EditorApplication::SaveWorld();
      }
      if (Input::IsKeyDown(KeyCode::W)) {
        EditorApplication::GetInstance()->Exit();  
      }
      if (Input::IsKeyDown(KeyCode::D)) {
        EditorApplication::DuplicateEntity();
      }
    }
    
    if (Input::IsKeyDown(KeyCode::F1)) {
      main_window->SetWindowMode(main_window->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F2)) {
      RenderEngine::SetVSyncMode(
        RenderEngine::GetVSyncMode() == VSyncMode::DontSync
          ? VSyncMode::EveryVBlank
          : VSyncMode::DontSync
      );
    }
    if (Input::IsKeyDown(KeyCode::F3)) {
      RenderGizmos::SetShouldDrawGrid(!RenderGizmos::GetShouldDrawGrid());
    }
    if (Input::IsKeyDown(KeyCode::F4)) {
      RenderGizmos::SetShouldDrawMeshBounds(!RenderGizmos::GetShouldDrawMeshBounds());
    }
    if (Input::IsKeyDown(KeyCode::F5)) {
      if (Engine::GetEngineState() == EngineState::Editor) {
        EditorApplication::EnterRuntime();
      } else {
        EditorApplication::ExitRuntime();
      }
    }
    
    if (Input::IsKeyDown(KeyCode::Delete)) {
      EditorApplication::DestroyEntity();
    }
    if (Input::IsKeyDown(KeyCode::Q)) {
      EditorUI::SetTransformationTool(RenderGizmoType::Translate);
    }
    if (Input::IsKeyDown(KeyCode::W)) {
      EditorUI::SetTransformationTool(RenderGizmoType::Rotate);
    }
    if (Input::IsKeyDown(KeyCode::E)) {
      EditorUI::SetTransformationTool(RenderGizmoType::Scale);
    }
    if (Input::IsKeyDown(KeyCode::R)) {
      EditorUI::SetTransformationMode(UI::GizmoMode::Local);
    }
    if (Input::IsKeyDown(KeyCode::T)) {
      EditorUI::SetTransformationMode(UI::GizmoMode::World);
    }
  }
  
}
