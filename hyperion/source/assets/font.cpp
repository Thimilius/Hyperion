//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/font.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Font::Font(AssetInfo info, uint32 size, float32 baseline_offset, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs) : Font(info) {
    m_size = size;
    m_baseline_offset = baseline_offset;
    m_character_set = character_set;
    m_font_atlas = font_atlas;
    m_special_glyphs = special_glyphs;
  }

  //--------------------------------------------------------------
  const FontGlyph &Font::GetGlyph(uint32 codepoint) const {
    return m_font_atlas->GetElement(codepoint).payload;
  }

  //--------------------------------------------------------------
  const FontAtlasElement &Font::GetElement(uint32 codepoint) const {
    return m_font_atlas->GetElement(codepoint);
  }

  //--------------------------------------------------------------
  TextSize Font::GetTextSize(const Array<uint32> &codepoints, uint32 codepoint_offset, float32 scale, bool8 line_only) const {
    float32 text_width = -FLT_MAX;
    float32 text_height = static_cast<float32>(m_size);
    float32 current_line_width = 0.0f;
    float32 current_line_height = 0.0f;

    for (uint32 i = codepoint_offset; i < codepoints.GetLength(); i++) {
      uint32 codepoint = codepoints[i];
      // We first handle the special characters.
      switch (codepoint) {
        case ' ': current_line_width += GetSpecialGlyphs().space.advance * scale;
          continue;
        case '\t': current_line_width += GetSpecialGlyphs().space.advance * 4 * scale;
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
            text_height += m_size; 
            continue;
          }
        }
      }

      FontGlyph glyph = GetGlyph(codepoint);
      current_line_width += glyph.advance * scale;
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
    text_size.baseline_offset = m_baseline_offset;
    return text_size;
  }

}
