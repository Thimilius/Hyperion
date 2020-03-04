#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/rendering/texture_2d.hpp"

namespace Hyperion::Rendering {

    struct FontGlyph {
        char character;

        Ref<Texture2D> texture;

        Vec2 size;
        Vec2 bearing;
        u32 advance;
    };

    class Font : public Asset {
    private:
        u32 m_size;
        Map<u32, FontGlyph> m_glyphs;
    public:
        inline AssetType GetAssetType() const { return AssetType::Font; };

        inline u32 GetSize() const { return m_size; }
        inline FontGlyph GetGlyph(u32 c) const { return m_glyphs.at(c); }

        static Ref<Font> Create(const String &path, u32 size);
    private:
        Font(const String &path, u32 size);

        static void Init();

        friend class Hyperion::Engine;
    };

}