//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class TextUtilities final {
  public:
    static TextSize GetTextSize(Font *font, const Array<uint32> &codepoints, uint32 codepoint_offset, float32 scale, bool8 line_only);
    
    static Vector2 GetPosition(UI::TextAlignment text_alignment, TextSize line_size, TextSize full_size, Rect rect, Vector2 offset = Vector2());
    static Vector2 GetCursorPosition(
      Font *font,
      const Array<uint32> &codepoints,
      float32 scale,
      UI::TextAlignment text_alignment,
      Rect rect,
      Vector2Int cursor_position
    );
  private:
    static TextSize GetTextSize(Font *font, const Array<uint32> &codepoints, uint32 codepoint_offset, uint32 codepoint_size, float32 scale, bool8 line_only);
  private:
    TextUtilities() = delete;
    ~TextUtilities() = delete;
  };
  
}
  