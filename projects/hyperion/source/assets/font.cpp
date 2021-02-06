#include "hyppch.hpp"

#include "hyperion/assets/font.hpp"

#include "hyperion/modules/freetype/freetype_font_loader.hpp"

namespace Hyperion {

    Font::Font(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas) {
        m_size = size;
        m_character_set = character_set;
        m_font_atlas = font_atlas;
    }

    const FontGlyph &Font::GetGlyph(uint32 codepoint) const {
        return m_font_atlas->GetElement(codepoint).payload;
    }

    const FontAtlasElement &Font::GetElement(uint32 codepoint) const {
        return m_font_atlas->GetElement(codepoint);
    }

    float32 Font::GetTextWidth(const String &text, float32 scale) const {
        float32 width = 0.0f;
        Vector<uint32> codepoints = StringUtils::GetCodepointsUtf8(text);
        uint64 codepoints_count = codepoints.size();

        for (uint32 i = 0; i < codepoints_count; i++) {
            FontGlyph glyph = GetGlyph(codepoints[i]);
            width += glyph.advance * scale;
        }
        return width;
    }

    void Font::OnDestroy() {
        Destroy(m_font_atlas);
    }

    Font *Font::Create(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas) {
        return new Font(size, character_set, font_atlas);
    }

    Font *Font::Create() {
        return new Font();
    }

    void FontLoader::Initialize() {
        s_font_loader = new FreetypeFontLoader();
        s_font_loader->Initialize();
    }

    void FontLoader::Shutdown() {
        s_font_loader->Shutdown();
        delete s_font_loader;
    }

}