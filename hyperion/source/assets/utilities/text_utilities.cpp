//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/text_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  TextSize TextUtilities::GetTextSize(Font *font, const Array<uint32> &codepoints, uint32 codepoint_offset, float32 scale, bool8 line_only) {
    return GetTextSize(font, codepoints, codepoint_offset, static_cast<uint32>(codepoints.GetLength()), scale, line_only);
  }
  
  //--------------------------------------------------------------
  Vector2 TextUtilities::GetPosition(UI::TextAlignment text_alignment, TextSize line_size, TextSize full_size, Rect rect) {
    Vector2 position = Vector2();

    switch (text_alignment) {
      case UI::TextAlignment::TopLeft: {
        position = Vector2(rect.position.x, rect.GetMax().y - line_size.height);
        break;
      }
      case UI::TextAlignment::TopCenter: {
        position = Vector2(rect.GetCenter().x - (line_size.width / 2.0f), rect.GetMax().y - line_size.height);
        break;
      }
      case UI::TextAlignment::TopRight: {
        Vector2 max = rect.GetMax();
        position = Vector2(max.x - line_size.width, max.y - line_size.height);
        break;
      }
      case UI::TextAlignment::MiddleLeft: {
        position = Vector2(rect.position.x, rect.GetCenter().y + (full_size.height / 2.0f) - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::MiddleCenter: {
        Vector2 center = rect.GetCenter();
        position = Vector2(center.x - (line_size.width / 2.0f), center.y + (full_size.height / 2.0f) - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::MiddleRight: {
        position = Vector2(rect.GetMax().x - line_size.width, rect.GetCenter().y + (full_size.height / 2.0f) - line_size.height  + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomLeft: {
        position = Vector2(rect.position.x, rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomCenter: {
        position = Vector2(rect.GetCenter().x - (line_size.width / 2.0f), rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      case UI::TextAlignment::BottomRight: {
        position = Vector2(rect.GetMax().x - line_size.width, rect.position.y + full_size.height - line_size.height + line_size.baseline_offset);
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    return Vector3(position, 0.0f);
  }

  //--------------------------------------------------------------
  Vector2 TextUtilities::GetCursorPosition(
    Font *font,
    const Array<uint32> &codepoints,
    float32 scale,
    UI::TextAlignment text_alignment,
    Rect rect,
    Vector2Int cursor_position
  ) {
    TextSize full_text_size = GetTextSize(font, codepoints, 0, scale, false);
    TextSize first_line_text_size = GetTextSize(font, codepoints, 0, scale, true);

    TextSize text_size = GetTextSize(font, codepoints, 0, static_cast<uint32>(cursor_position.x), scale, true);
    Vector2 text_position = GetPosition(text_alignment, first_line_text_size, full_text_size, rect);

    text_position.x += text_size.width;
    text_position.y -= font->GetBaselineOffset();
    
    return text_position;
  }

  //--------------------------------------------------------------
  TextSize TextUtilities::GetTextSize(
    Font *font,
    const Array<uint32> &codepoints,
    uint32 codepoint_offset,
    uint32 codepoint_size,
    float32 scale,
    bool8 line_only
  ) {
    float32 text_width = -FLT_MAX;
    float32 text_height = static_cast<float32>(font->GetSize());
    float32 current_line_width = 0.0f;
    float32 current_line_height = 0.0f;

    for (uint32 i = codepoint_offset; i < codepoint_size; i++) {
      uint32 codepoint = codepoints[i];
      // We first handle the special characters.
      switch (codepoint) {
        case ' ': current_line_width += static_cast<float32>(font->GetSpecialGlyphs().space.advance) * scale;
        continue;
        case '\t': current_line_width += static_cast<float32>(font->GetSpecialGlyphs().space.advance) * 4 * scale;
        continue; // Tab is equivalent to 4 whitespaces.
        case '\r': continue; // Carriage return gets just straight up ignored. 
        case '\n': {
          // If we have multiple lines, we are essentially searching for the widest line.
          if (current_line_width > text_width) {
            text_width = current_line_width;
          }
          current_line_width = 0.0f;

          if (line_only) {
            goto end;
          } else {
            text_height += static_cast<float32>(font->GetSize()); 
            continue;
          }
        }
      }

      FontGlyph glyph = font->GetGlyph(codepoint);
      current_line_width += static_cast<float32>(glyph.advance) * scale;
      float32 glyph_height = glyph.bearing.y * scale;
      if (glyph_height > current_line_height) {
        current_line_height = glyph_height;
      }
    }

    end:
      if (current_line_width > text_width) {
        text_width = current_line_width;
      }

    TextSize text_size = { };
    text_size.width = text_width;
    text_size.height = text_height;
    text_size.baseline_offset = font->GetBaselineOffset();
    return text_size;
  }

}
  