#include "hyppch.hpp"

#include "hyperion/modules/freetype/freetype_font_loader.hpp"

#include <ft2build.h>
#include <freetype/freetype.h>

namespace Hyperion {

    Font *FreetypeFontLoader::LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) {
        FT_Face font_face;
        if (FT_New_Face(s_freetype_library, path.c_str(), 0, &font_face)) {
            HYP_LOG_ERROR("Engine", "Failed to load font from path: '{}'!", path);
            return nullptr;
        }

        FT_Set_Pixel_Sizes(font_face, 0, font_size);

        uint32 TEXTURE_ATLAS_SIZE = 1024;
        uint32 TEXTURE_ATLAS_PADDING = 4;
        TextureAtlasPacker<uint32, FontGlyph> texture_atlas_packer(TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_PADDING, font_size);

        FT_UInt index;
        FT_ULong character = FT_Get_First_Char(font_face, &index);
        while (true) {
            // Skip characters that are not part of the set we want to load.
            bool should_load_character = false;
            switch (character_set) {
                case FontCharacterSet::ASCII: should_load_character = (character >= 0x20 && character <= 0x7F); break;
                case FontCharacterSet::LatinSupplement: should_load_character = (character >= 0x20 && character <= 0xFF); break;
                case FontCharacterSet::All: should_load_character = true; break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }

            if (should_load_character) {
                int32 error = FT_Load_Char(font_face, character, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT);
                if (error) {
                    // NOTE: Some fonts contain characters that freetype can not load and returns -1 as a non defined error code.
                    // We just ignore all those for now.
                    if (error == -1) {
                        goto continue_loop;
                    }

                    HYP_LOG_ERROR("Engine", "Failed to load font glyph '{}'!", character);
                    break;
                }

                // We need to make sure the bitmap of the character is actually valid.
                // One very weird thing is that we have to check that the bitmap height is not greater than the font size.
                uint32 bitmap_width = font_face->glyph->bitmap.width;
                uint32 bitmap_height = font_face->glyph->bitmap.rows;
                if (bitmap_width == 0 || bitmap_height == 0) {
                    goto continue_loop;
                }
                if (bitmap_height > font_size) {
                    HYP_LOG_WARN("Engine", "The font glyph '{}' is greater than the font size!", character);
                    goto continue_loop;
                }
                uint8 *bitmap_buffer = font_face->glyph->bitmap.buffer;
                if (bitmap_buffer == nullptr) {
                    goto continue_loop;
                }

                FontGlyph glyph;
                glyph.codepoint = static_cast<uint32>(character);
                glyph.size = Vec2(static_cast<float32>(bitmap_width), static_cast<float32>(bitmap_height));
                glyph.bearing = Vec2(static_cast<float32>(font_face->glyph->bitmap_left), static_cast<float32>(font_face->glyph->bitmap_top));
                glyph.advance = font_face->glyph->advance.x >> 6;

                if (!texture_atlas_packer.AddElement(glyph.codepoint, glyph, bitmap_width, bitmap_height, bitmap_buffer)) {
                    break;
                }
            }

        continue_loop:
            character = FT_Get_Next_Char(font_face, character, &index);
            if (!index) {
                break;
            }
        }
        FT_Done_Face(font_face);

        FontAtlas *font_atlas = texture_atlas_packer.CreateAtlas();
        return Font::Create(font_size, character_set, font_atlas);
    }

    void FreetypeFontLoader::Initialize() {
        if (FT_Init_FreeType(&s_freetype_library)) {
            HYP_LOG_ERROR("Engine", "Failed to initialize freetype library!");
        }
    }

    void FreetypeFontLoader::Shutdown() {
        FT_Done_FreeType(s_freetype_library);
    }

}