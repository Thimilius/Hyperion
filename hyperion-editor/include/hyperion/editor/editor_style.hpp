//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/color.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorStyle final {
  public:
    inline static const uint32 FONT_SIZE = 12;

    inline static const uint32 HEADER_WIDTH = 25;
    inline static const uint32 FOOTER_WIDTH = 25;
    inline static const uint32 BOTTOM_BAR_WIDTH = 300;
    inline static const uint32 LEFT_BAR_WIDTH = 400;
    inline static const uint32 LEFT_TOP_BAR_HEIGHT = 400;
    inline static const uint32 LEFT_BOTTOM_BAR_HEIGHT = 400;

    inline static const Color COLOR_NORMAL = Color(0.25f, 0.25f, 0.25f, 1.0f);
    inline static const Color COLOR_NORMAL_DARK = Color(0.2f, 0.2f, 0.2f, 1.0f);
    inline static const Color COLOR_HIGHLIGHT = Color(0.0f, 0.4f, 0.8f, 1.0f);
  private:
    EditorStyle() = delete;
    ~EditorStyle() = delete;
  };

}
