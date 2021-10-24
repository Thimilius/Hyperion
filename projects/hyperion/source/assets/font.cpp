//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/font.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Font::Font(AssetInfo info, uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs) : Font(info) {
        m_size = size;
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
    TextSize Font::GetTextSize(const String &text, float32 scale) const {
        float32 max_width = -FLT_MAX;
        float32 max_height = -FLT_MAX;
        float32 max_glyph_baseline_offset = -FLT_MAX;
        float32 current_width = 0.0f;
        float32 current_height = 0.0f;
        float32 current_glyph_baseline_offset = 0.0f;

        Array<uint32> codepoints = StringUtils::GetCodepointsUtf8(text);
        for (uint32 codepoint : codepoints) {
            // We first handle the special characters.
            switch (codepoint) {
                case ' ': current_width += GetSpecialGlyphs().space.advance * scale; continue;
                case '\t': current_width += GetSpecialGlyphs().space.advance * 4 * scale; continue; // Tab is equivalent to 4 whitespaces.
                case '\r': continue; // Carriage return gets just straight up ignored. 
                case '\n':
                {
                    // If we have multiple lines, we are essentially searching for the widest line.
                    if (current_width > max_width) {
                        max_width = current_width;
                    }
                    current_width = 0.0f;
                    continue;
                }
            }

            FontGlyph glyph = GetGlyph(codepoint);
            current_width += glyph.advance * scale;
            current_height = glyph.bearing.y * scale;
            if (current_height > max_height) {
                max_height = current_height;
            }
            current_glyph_baseline_offset = (glyph.size.y - glyph.bearing.y) * scale;
            if (current_glyph_baseline_offset > max_glyph_baseline_offset) {
                max_glyph_baseline_offset = current_glyph_baseline_offset;
            }
        }

        if (current_width > max_width) {
            max_width = current_width;
        }
        if (current_height > max_height) {
            max_height = current_height;
        }

        TextSize text_size;
        text_size.width = max_width;
        text_size.height = max_height;
        text_size.baseline_offset = max_glyph_baseline_offset;
        return text_size;
    }

}