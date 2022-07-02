//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <functional>

//---------------------- Library Includes ----------------------
#include <hyperion/common.hpp>
#include <hyperion/core/app/input_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  using EditorKeyboardShortcutCallback = std::function<void()>; 
  
  struct EditorKeyboardShortcut {
    String name;
    KeyModifier key_modifier;
    KeyCode key_code;
    EditorKeyboardShortcutCallback callback;
  };
  
  class EditorInput final {
  public:
    static void Initialize();
    static void Update();
  private:
    EditorInput() = delete;
    ~EditorInput() = delete;
  private:
    static void AddShortcut(const String &name, KeyModifier modifier, KeyCode code, EditorKeyboardShortcutCallback callback);
  private:
    inline static Array<EditorKeyboardShortcut> s_shortcuts;
  };

}
