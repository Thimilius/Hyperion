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

        const uint32 texture_atlas_size = 512;
        const uint32 padding = 4;
        Vector<uint8> texture_atlas(texture_atlas_size * texture_atlas_size);
        uint8 *texture_atlas_buffer = texture_atlas.data();
        uint32 texture_atlas_column = 0;
        uint32 texture_atlas_row = 0;

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

                if (font_face->glyph->bitmap.width == 0 || font_face->glyph->bitmap.rows == 0) {
                    goto continue_loop;
                }
                uint8 *character_bitmap_buffer = font_face->glyph->bitmap.buffer;
                if (character_bitmap_buffer == nullptr) {
                    goto continue_loop;
                }

                // We might already have the glyph loaded for some reason!?
                if (glyphs.find(character) != glyphs.end()) {
                    goto continue_loop;
                }

                uint32 bitmap_width = font_face->glyph->bitmap.width;
                uint32 bitmap_height = font_face->glyph->bitmap.rows;

                FontGlyph glyph;
                glyph.codepoint = static_cast<uint32>(character);
                glyph.size = Vec2(static_cast<float32>(bitmap_width), static_cast<float32>(bitmap_height));
                glyph.bearing = Vec2(static_cast<float32>(font_face->glyph->bitmap_left), static_cast<float32>(font_face->glyph->bitmap_top));
                glyph.advance = font_face->glyph->advance.x >> 6;

                // Pack the bitmap into the texture atlas...
                {
                    // Make sure we have enough space in the texture atlas to put the character bitmap into it.
                    if ((texture_atlas_column + bitmap_width + padding) >= texture_atlas_size) {
                        texture_atlas_column = 0;
                        texture_atlas_row += size + padding;

                        if ((texture_atlas_row + bitmap_height + padding) >= texture_atlas_size) {
                            HYP_LOG_WARN("Engine", "Texture atlas is to small to fit in all font characters!");
                            break;
                        }
                    }

                    // Create the uvs that point to the corresponding location inside the texture atlas.
                    // We have to remember that the origin of the font atlas is at the top left corner just like a regular image.
                    // This means the y-coordinate (v) of the uvs starts at 1 and goes down instead of starting at 0 and going up.
                    {
                        float32 u = static_cast<float32>(texture_atlas_column);
                        float32 v = static_cast<float32>(texture_atlas_size - texture_atlas_row);
                        float32 divide = static_cast<float32>(texture_atlas_size);

                        float32 left = u / divide;
                        float32 right = (u + bitmap_width) / divide;
                        float32 top = v / divide;
                        float32 bottom = (v - bitmap_height) / divide;

                        glyph.uv[static_cast<uint32>(FontGlyph::UVCorner::TopLeft)] = Vec2(left, top);
                        glyph.uv[static_cast<uint32>(FontGlyph::UVCorner::TopRight)] = Vec2(right, top);
                        glyph.uv[static_cast<uint32>(FontGlyph::UVCorner::BottomRight)] = Vec2(right, bottom);
                        glyph.uv[static_cast<uint32>(FontGlyph::UVCorner::BottomLeft)] = Vec2(left, bottom);
                    }

                    // Now put the actual bitmap data into the texture atlas.
                    // At this point we know we have enough space and we should therefore not overflow the buffer.
                    uint32 texture_data_index = texture_atlas_column + (texture_atlas_row * texture_atlas_size);
                    for (uint32 y = 0; y < bitmap_height; y++) {
                        for (uint32 x = 0; x < bitmap_width; x++) {
                            texture_atlas_buffer[texture_data_index + x] = character_bitmap_buffer[x + (y * bitmap_width)];
                        }
                        texture_data_index += texture_atlas_size;
                    }
                    texture_atlas_column += bitmap_width + padding;
                }

                glyphs[static_cast<uint32>(character)] = glyph;
            }

        continue_loop:
            character = FT_Get_Next_Char(font_face, character, &index);
            if (!index) {
                break;
            }
        }
        FT_Done_Face(font_face);

        Rendering::TextureParameters texture_parameters;
        texture_parameters.use_mipmaps = false;
        Texture2D *texture = Texture2D::Create(texture_atlas_size, texture_atlas_size, Rendering::TextureFormat::R8, texture_parameters, texture_atlas);

        return Font::Create(size, character_set, glyphs, texture);
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