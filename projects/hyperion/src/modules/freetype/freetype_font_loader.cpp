#include "hyppch.hpp"

#include "hyperion/modules/freetype/freetype_font_loader.hpp"

#include <ft2build.h>
#include <freetype/freetype.h>

namespace Hyperion {

    Font *FreetypeFontLoader::LoadFont(const String &path, uint32 size, FontCharacterSet character_set) {
        Map<uint32, FontGlyph> glyphs;

        FT_Face font_face;
        if (FT_New_Face(s_freetype_library, path.c_str(), 0, &font_face)) {
            HYP_LOG_ERROR("Engine", "Failed to load font from path: '{}'!", path);
        }

        FT_Set_Pixel_Sizes(font_face, 0, size);

        Rendering::TextureParameters texture_parameters;
        texture_parameters.use_mipmaps = false;

        FT_UInt index;
        FT_ULong character = FT_Get_First_Char(font_face, &index);
        while (true) {
            int32 error = FT_Load_Char(font_face, character, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
            if (error) {
                // NOTE: Some fonts contain characters that freetype can not load and returns -1 as a non defined error code
                // We just ignore all those for now
                if (error == -1) {
                    character = FT_Get_Next_Char(font_face, character, &index);
                    continue;
                }

                HYP_LOG_ERROR("Engine", "Failed to load font glyph '{}'!", character);
            }

            // Skip characters that are not part of the set we want to load
            bool should_load_character = false;
            switch (character_set) {
                case FontCharacterSet::ASCII: should_load_character = (character >= 0x20 && character <= 0x7F); break;
                case FontCharacterSet::LatinSupplement: should_load_character = (character >= 0x20 && character <= 0xFF); break;
                case FontCharacterSet::All: should_load_character = true; break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }

            if (should_load_character) {
                if (font_face->glyph->bitmap.width == 0 || font_face->glyph->bitmap.rows == 0) {
                    goto continue_loop;
                }
                uint8 *buffer = font_face->glyph->bitmap.buffer;
                if (buffer == nullptr) {
                    goto continue_loop;
                }

                uint32 bitmap_width = font_face->glyph->bitmap.width;
                uint32 bitmap_height = font_face->glyph->bitmap.rows;
                uint32 size = bitmap_width * bitmap_height;

                // FIXME: This is an unecessary copy!
                Vector<uint8> pixels(buffer, buffer + size);
                Texture2D *texture = Texture2D::Create(bitmap_width, bitmap_height, Rendering::TextureFormat::R8, texture_parameters, pixels);

                FontGlyph glyph;
                glyph.codepoint = static_cast<uint32>(character);
                glyph.texture = texture;
                glyph.size = Vec2(static_cast<float32>(bitmap_width), static_cast<float32>(bitmap_height));
                glyph.bearing = Vec2(static_cast<float32>(font_face->glyph->bitmap_left), static_cast<float32>(font_face->glyph->bitmap_top));

                glyph.advance = font_face->glyph->advance.x >> 6;

                glyphs[static_cast<uint32>(character)] = glyph;
            }

        continue_loop:
            character = FT_Get_Next_Char(font_face, character, &index);
            if (!index) {
                break;
            }
        }

        FT_Done_Face(font_face);

        return Font::Create(size, character_set, glyphs);
    }

    void FreetypeFontLoader::Init() {
        if (FT_Init_FreeType(&s_freetype_library)) {
            HYP_LOG_ERROR("Engine", "Failed to initialize freetype library!");
        }
    }

    void FreetypeFontLoader::Shutdown() {
        FT_Done_FreeType(s_freetype_library);
    }

}