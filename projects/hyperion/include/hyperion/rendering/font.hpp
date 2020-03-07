#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/rendering/texture_2d.hpp"

struct FT_LibraryRec_;
namespace Hyperion {
    class Engine;
}

namespace Hyperion::Rendering {

    struct FontGlyph {
        u32 codepoint;

        Ref<Texture2D> texture;

        Vec2 size;
        Vec2 bearing;
        u32 advance;
    };

    class Font : public Asset {
    private:
        u32 m_size;
        Map<u32, FontGlyph> m_glyphs;

        inline static FT_LibraryRec_ *s_freetype_library;
    public:
        inline AssetType GetAssetType() const { return AssetType::Font; };

        inline u32 GetSize() const { return m_size; }
        FontGlyph GetGlyph(u32 codepoint) const;

        static Ref<Font> Create(const String &path, u32 size);
    private:
        Font(const String &path, u32 size);

        static void Init();

        friend class Hyperion::Engine;
    };

}