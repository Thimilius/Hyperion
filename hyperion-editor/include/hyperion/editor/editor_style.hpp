//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/color.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorStyle final {
  public:
    inline static constexpr uint32 FONT_SIZE = 12;

    inline static constexpr uint32 HEADER_HEIGHT = 25;
    inline static constexpr uint32 FOOTER_HEIGHT = 25;

    inline static const Color COLOR_NORMAL = Color(0.25f, 0.25f, 0.25f, 1.0f);
    inline static const Color COLOR_NORMAL_DARK = Color(0.2f, 0.2f, 0.2f, 1.0f);
    inline static const Color COLOR_HIGHLIGHT = Color(0.0f, 0.4f, 0.8f, 1.0f);
    inline static const Color COLOR_HIGHLIGHT_HOVERED = Color(0.0f, 0.5f, 0.9f, 1.0f);
    inline static const Color COLOR_HIGHLIGHT_PRESSED = Color(0.0f, 0.3f, 0.7f, 1.0f);
    inline static const Color COLOR_HOVERED = Color(0.3f, 0.3f, 0.3f, 1.0f);
    inline static const Color COLOR_PRESSED = Color(0.15f, 0.15f, 0.15f, 1.0f);
    inline static const Color COLOR_FOCUSED = Color(0.0f, 0.2f, 0.4f, 1.0f);
    inline static const Color COLOR_DISABLED = Color(0.75f, 0.75f, 0.75f, 1.0f);
    inline static const Color COLOR_PLAYMODE_OVERLAY = Color(1.0f, 0.75f, 0.75f, 1.0f);
  private:
    EditorStyle() = delete;
    ~EditorStyle() = delete;
  };

}
