//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/freetype/freetype_font_loader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/texture_atlas_packer.hpp"

//---------------------- Library Includes ----------------------
#include <ft2build.h>
#include <freetype/freetype.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Font *FreetypeFontLoader::LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) {
    FT_Face font_face;
    if (FT_New_Face(s_freetype_library, path.c_str(), 0, &font_face)) {
      HYP_LOG_ERROR("Engine", "Failed to load font from path: '{}'!", path);
      return nullptr;
    }

    FT_Set_Pixel_Sizes(font_face, 0, font_size);

    uint32 TEXTURE_ATLAS_SIZE = 1024;
    uint32 TEXTURE_ATLAS_PADDING = 4;
    // NOTE: Setting the pixel size does not guarantee that the bitmaps are at most of that height.
    // We first have to compute this actual height and pass it to the atlas packer so that every bitmap can fit properly.
    uint32 actual_font_height = static_cast<uint32>(Math::Round(
      static_cast<float32>((font_face->bbox.yMax - font_face->bbox.yMin) * font_size) / static_cast<float32>(font_face->units_per_EM)));
    TextureAtlasPacker<uint32, FontGlyph> texture_atlas_packer(TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_SIZE, TEXTURE_ATLAS_PADDING, actual_font_height);

    SpecialFontGlyphs special_glyphs;

    FT_UInt index;
    FT_ULong character = FT_Get_First_Char(font_face, &index);
    while (true) {
      // Skip characters that are not part of the set we want to load.
      bool should_load_character = false;
      switch (character_set) {
        case FontCharacterSet::ASCII: should_load_character = (character >= 0x20 && character <= 0x7F);
          break;
        case FontCharacterSet::LatinSupplement: should_load_character = (character >= 0x20 && character <= 0xFF);
          break;
        case FontCharacterSet::All: should_load_character = true;
          break;
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
          break;
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

        // There are special characters that have no bitmap but should still be loaded.
        bool is_special_character = character == ' ';

        uint32 bitmap_width = font_face->glyph->bitmap.width;
        uint32 bitmap_height = font_face->glyph->bitmap.rows;
        uint8 *bitmap_buffer = font_face->glyph->bitmap.buffer;
        if (!is_special_character) {
          // We need to make sure the bitmap of the character is actually valid.
          if (bitmap_width == 0 || bitmap_height == 0 || bitmap_buffer == nullptr) {
            goto continue_loop;
          }
        }

        FontGlyph glyph;
        glyph.codepoint = static_cast<uint32>(character);
        glyph.size = Vector2(static_cast<float32>(bitmap_width), static_cast<float32>(bitmap_height));
        glyph.bearing = Vector2(static_cast<float32>(font_face->glyph->bitmap_left), static_cast<float32>(font_face->glyph->bitmap_top));
        glyph.advance = font_face->glyph->advance.x >> 6;

        if (is_special_character) {
          if (character == ' ') {
            special_glyphs.space = glyph;
          }
        } else {
          if (!texture_atlas_packer.AddElement(glyph.codepoint, glyph, bitmap_width, bitmap_height, bitmap_buffer)) {
            break;
          }
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
    return AssetManager::CreateFont(font_size, character_set, font_atlas, special_glyphs);
  }

  //--------------------------------------------------------------
  void FreetypeFontLoader::Initialize() {
    if (FT_Init_FreeType(&s_freetype_library)) {
      HYP_LOG_ERROR("Engine", "Failed to initialize freetype library!");
    }
  }

  //--------------------------------------------------------------
  void FreetypeFontLoader::Shutdown() {
    FT_Done_FreeType(s_freetype_library);
  }

}
