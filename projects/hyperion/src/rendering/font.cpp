#include "hyppch.hpp"

#include "hyperion/rendering/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Hyperion::Rendering {

    FontGlyph Font::GetGlyph(u32 codepoint) const {
        auto glyph = m_glyphs.find(codepoint);
        if (glyph != m_glyphs.end()) {
            return (*glyph).second;
        } else {
            // FIXME: What happens if the font does not have '?' as a glyph?
            return m_glyphs.at('?');
        }
    }

    Ref<Font> Font::Create(const String &path, u32 size, FontCharacterSet character_set) {
        return Ref<Font>(new Font(path, size, character_set));
    }

    // TODO: Generate font atlas instead of individual textures

    Font::Font(const String &path, u32 size, FontCharacterSet character_set) {
        m_size = size;
        m_character_set = character_set;

        FT_Face font_face;
        if (FT_New_Face(s_freetype_library, path.c_str(), 0, &font_face)) {
            HYP_LOG_ERROR("Engine", "Failed to load font from path: '{}'!", path);
        }

        FT_Set_Pixel_Sizes(font_face, 0, size);

        TextureParameters texture_parameters;
        texture_parameters.use_mipmaps = false;

        FT_UInt index;
        FT_ULong character = FT_Get_First_Char(font_face, &index);
        while (true) {
            if (FT_Load_Char(font_face, character, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) {
                HYP_LOG_ERROR("Engine", "Failed to load font glyph '{}'!", character);
            }

            // Skip characters that are not part of the set we want to load
            bool load_character = false;
            switch (character_set) {
                case FontCharacterSet::ASCII: load_character = (character >= 0x20 && character <= 0x7F); break;
                case FontCharacterSet::LatinSupplement: load_character = (character >= 0x20 && character <= 0xFF); break;
                case FontCharacterSet::All: load_character = true; break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }

            if (load_character) {
                u32 bitmap_width = font_face->glyph->bitmap.width == 0 ? 1 : font_face->glyph->bitmap.width;
                u32 bitmap_height = font_face->glyph->bitmap.width == 0 ? 1 : font_face->glyph->bitmap.rows;
                auto *buffer = font_face->glyph->bitmap.buffer;

                Ref<Texture2D> texture = Texture2D::Create(bitmap_width, bitmap_height, TextureFormat::R8, texture_parameters, buffer);

                FontGlyph glyph;
                glyph.codepoint = static_cast<u32>(character);
                glyph.texture = texture;
                glyph.size = Vec2(static_cast<f32>(bitmap_width), static_cast<f32>(bitmap_height));
                glyph.bearing = Vec2(static_cast<f32>(font_face->glyph->bitmap_left), static_cast<f32>(font_face->glyph->bitmap_top));
                glyph.advance = font_face->glyph->advance.x >> 6;

                m_glyphs[static_cast<u32>(character)] = glyph;
            }

            character = FT_Get_Next_Char(font_face, character, &index);
            if (!index) {
                break;
            }
        }

        FT_Done_Face(font_face);
    }

    void Font::Init() {
        if (FT_Init_FreeType(&s_freetype_library)) {
            HYP_LOG_ERROR("Engine", "Failed to initialize freetype library!");
        }
    }

}