//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/font.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Font::Font(
    AssetMetadata metadata,
    uint32 size,
    float32 baseline_offset,
    FontCharacterSet character_set,
    FontAtlas *font_atlas,
    SpecialFontGlyphs special_glyphs
  ) : Font(std::move(metadata)) {
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

}
