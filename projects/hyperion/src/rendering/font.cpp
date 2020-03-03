#include "hyppch.hpp"

#include "hyperion/rendering/font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Hyperion::Rendering {

    bool g_freetype_library_initialized = false;
    FT_Library g_freetype_library;

    Ref<Font> Font::Create(const String &path, u32 size) {
        return Ref<Font>(new Font(path, size));
    }

    // TODO: Generate font atlas instead of individual textures

    Font::Font(const String &path, u32 size) {
        m_size = size;

        FT_Face font_face;
        if (FT_New_Face(g_freetype_library, path.c_str(), 0, &font_face)) {
            HYP_LOG_ERROR("Engine", "Failed to load font from path: '{}'!", path);
        }

        FT_Set_Pixel_Sizes(font_face, 0, size);

        TextureParameters texture_parameters;
        texture_parameters.filter = TextureFilter::Bilinear;

        FT_UInt index;
        FT_ULong character = FT_Get_First_Char(font_face, &index);
        while (true) {
            if (FT_Load_Char(font_face, character, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_LIGHT)) {
                HYP_LOG_ERROR("Engine", "Failed to load font glyph '{}'!", character);
            }

            u32 bitmap_width = font_face->glyph->bitmap.width == 0 ? 1 : font_face->glyph->bitmap.width;
            u32 bitmap_height = font_face->glyph->bitmap.width == 0 ? 1 : font_face->glyph->bitmap.rows;
            auto *buffer = font_face->glyph->bitmap.buffer;

            Ref<Texture2D> texture = Texture2D::Create(bitmap_width, bitmap_height, TextureFormat::R8, texture_parameters, buffer);

            FontGlyph glyph;
            glyph.character = (u32)character;
            glyph.texture = texture;
            glyph.size = Vec2((f32)bitmap_width, (f32)bitmap_height);
            glyph.bearing = Vec2((f32)font_face->glyph->bitmap_left, (f32)font_face->glyph->bitmap_top);
            glyph.advance = font_face->glyph->advance.x >> 6;

            m_glyphs[(u32)character] = glyph;

            character = FT_Get_Next_Char(font_face, character, &index);
            if (!index) {
                break;
            }
        }

        FT_Done_Face(font_face);
    }

    void Font::Init() {
        if (FT_Init_FreeType(&g_freetype_library)) {
            HYP_LOG_ERROR("Engine", "Failed to initialize freetype library!");
        }
    }

}