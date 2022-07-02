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
  void EditorInput::Initialize() {
    AddShortcut("New World", KeyModifier::Control, KeyCode::N, []() { EditorApplication::NewWorld(); });
    AddShortcut("Open World", KeyModifier::Control, KeyCode::O, []() { EditorApplication::OpenWorld(); });
    AddShortcut("Save World", KeyModifier::Control, KeyCode::S, []() { EditorApplication::SaveWorld(); });
    AddShortcut("Quit", KeyModifier::Control, KeyCode::W, []() { EditorApplication::GetInstance()->Exit(); });
    AddShortcut("Duplicate Entity", KeyModifier::Control, KeyCode::D, []() { EditorApplication::DuplicateEntity(); });

    AddShortcut("Toggle Window Mode", KeyModifier::None, KeyCode::F1, []() {
      Window *main_window = EditorApplication::GetInstance()->GetMainWindow();
      main_window->SetWindowMode(main_window->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    });
    AddShortcut("Toggle VSync", KeyModifier::None, KeyCode::F2, []() {
      RenderEngine::SetVSyncMode(RenderEngine::GetVSyncMode() == VSyncMode::DontSync ? VSyncMode::EveryVBlank : VSyncMode::DontSync);
    });
    AddShortcut("Toggle Grid", KeyModifier::None, KeyCode::F3, []() { RenderGizmos::SetShouldDrawGrid(!RenderGizmos::GetShouldDrawGrid()); });
    AddShortcut("Toggle Bounds", KeyModifier::None, KeyCode::F4, []() {
      RenderGizmos::SetShouldDrawMeshBounds(!RenderGizmos::GetShouldDrawMeshBounds());
    });
    AddShortcut("Enter/Exit Runtime", KeyModifier::None, KeyCode::F5, []() {
      if (Engine::GetEngineState() == EngineState::Editor) {
        EditorApplication::EnterRuntime();
      } else {
        EditorApplication::ExitRuntime();
      }
    });
    
    AddShortcut("Destroy Entity", KeyModifier::None, KeyCode::Delete, []() { EditorApplication::DestroyEntity(); });
    AddShortcut("Translate Tool", KeyModifier::None, KeyCode::Q, []() { EditorUI::SetTransformationTool(RenderGizmoType::Translate); });
    AddShortcut("Rotation Tool", KeyModifier::None, KeyCode::W, []() { EditorUI::SetTransformationTool(RenderGizmoType::Rotate); });
    AddShortcut("Scale Tool", KeyModifier::None, KeyCode::E, []() { EditorUI::SetTransformationTool(RenderGizmoType::Scale); });
    AddShortcut("Local Mode", KeyModifier::None, KeyCode::R, []() { EditorUI::SetTransformationMode(UI::GizmoMode::Local); });
    AddShortcut("Global Mode", KeyModifier::None, KeyCode::T, []() { EditorUI::SetTransformationMode(UI::GizmoMode::World); });
  }
  
  //--------------------------------------------------------------
  void EditorInput::Update() {
    // We do not want to check any keyboard shortcuts when we have a focused element.
    if (UI::UIImmediate::HasFocusedElement()) {
      return;
    }

    for (EditorKeyboardShortcut &shortcut : s_shortcuts) {
      KeyModifier modifier = shortcut.key_modifier;

      if (!Input::IsKeyDown(shortcut.key_code)) {
        continue;
      }

      bool8 has_none = modifier == KeyModifier::None;
      bool8 has_shift = (modifier & KeyModifier::Shift) == KeyModifier::Shift;
      bool8 has_control = (modifier & KeyModifier::Control) == KeyModifier::Control;
      bool8 has_alt = (modifier & KeyModifier::Alt) == KeyModifier::Alt;

      bool8 shift_hold = Input::IsKeyHold(KeyCode::Shift);
      bool8 control_hold = Input::IsKeyHold(KeyCode::Control);
      bool8 alt_hold = Input::IsKeyHold(KeyCode::Alt);
      
      if (has_shift && !shift_hold) {
        continue;
      }
      if (has_control && !control_hold) {
        continue;
      }
      if (has_alt && !alt_hold) {
        continue;
      }
      if (has_none && (shift_hold || control_hold || alt_hold)) {
        continue;
      }

      shortcut.callback();
    }
  }

  //--------------------------------------------------------------
  void EditorInput::AddShortcut(const String &name, KeyModifier modifier, KeyCode code, EditorKeyboardShortcutCallback callback) {
    EditorKeyboardShortcut shortcut;
    shortcut.name = name;
    shortcut.key_modifier = modifier;
    shortcut.key_code = code;
    shortcut.callback = std::move(callback);
    s_shortcuts.Add(shortcut);
  }

}
