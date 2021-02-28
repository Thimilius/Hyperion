//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/font.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/freetype/freetype_font_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Font::Font(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs) {
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
    Vec2 Font::GetTextSize(const String &text, float32 scale) const {
        float32 max_width = FLT_MIN;
        float32 max_height = FLT_MIN;
        float32 current_width = 0.0f;
        float32 current_height = 0.0f;

        Vector<uint32> codepoints = StringUtils::GetCodepointsUtf8(text);
        for (uint32 codepoint : codepoints) {
            // We first handle the special characters.
            switch (codepoint) {
                case ' ': current_width += GetSpecialGlyphs().space.advance * scale; continue;
                case '\t': current_width += GetSpecialGlyphs().space.advance * 4 * scale; continue; // Tab is equivalent to 4 whitespaces.
                case '\r': continue; // Carriage return gets just straight up ignored. 
                case '\n': {
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
        }

        if (current_width > max_width) {
            max_width = current_width;
        }
        if (current_height > max_height) {
            max_height = current_height;
        }
        return Vec2(max_width, max_height);
    }

    //--------------------------------------------------------------
    void Font::OnDestroy() {
        Destroy(m_font_atlas);
    }

    //--------------------------------------------------------------
    Font *Font::Create(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs) {
        return new Font(size, character_set, font_atlas, special_glyphs);
    }

    //--------------------------------------------------------------
    Font *Font::Create() {
        return new Font();
    }

    //--------------------------------------------------------------
    void FontLoader::Initialize() {
        s_font_loader = new FreetypeFontLoader();
        s_font_loader->Initialize();
    }

    //--------------------------------------------------------------
    void FontLoader::Shutdown() {
        s_font_loader->Shutdown();
        delete s_font_loader;
    }

}