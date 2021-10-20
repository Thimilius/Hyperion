#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/texture_atlas.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class FontCharacterSet {
        All,
        ASCII,
        LatinSupplement
    };

    struct FontGlyph {
        uint32 codepoint;

        Vector2 size;
        Vector2 bearing;
        uint32 advance;
    };

    struct SpecialFontGlyphs {
        FontGlyph space;
    };

    using FontAtlas = TextureAtlas<uint32, FontGlyph>;
    using FontAtlasElement = TextureAtlasElement<FontGlyph>;
    using FontAtlasMap = Map<uint32, FontAtlasElement>;

}