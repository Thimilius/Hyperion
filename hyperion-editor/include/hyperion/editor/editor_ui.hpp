//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/rect.hpp>
#include <hyperion/ui/immediate/ui_immediate_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorUI final {
  public:
    static void Initialize();
    static void Update();

    static RectInt GetPreviewRect();
  private:
    EditorUI() = delete;
    ~EditorUI() = delete;
  private:
    inline static UI::UIImmediateId s_preview_element;
  };

}
