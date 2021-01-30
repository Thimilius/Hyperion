#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/texture.hpp"

namespace Hyperion {

    enum class FontCharacterSet {
        All,
        ASCII,
        LatinSupplement
    };

    struct FontGlyph {
        enum class UVCorner {
            TopLeft,
            TopRight,
            BottomRight,
            BottomLeft
        };

        uint32 codepoint;

        // This stores the uvs that point inside the corresponding texture atlas of the font.
        // They are stored in the following clockwise order:
        //     uv[0] = Top Left
        //     uv[1] = Top Right
        //     uv[2] = Bottom Right
        //     uv[3] = Bottom Left
        Vec2 uv[4];

        Vec2 size;
        Vec2 bearing;
        uint32 advance;
    };

    class Font : public Asset {
        HYP_REFLECT(Asset);
    public:
        AssetType GetAssetType() const override { return AssetType::Font; }

        inline uint32 GetSize() const { return m_size; }
        inline Texture2D *GetTextureAtlas() const { return m_texture_atlas; }
        const FontGlyph &GetGlyph(uint32 codepoint) const;

        float32 GetTextWidth(const String &text, float32 scale) const;

        static Font *Create(uint32 size, FontCharacterSet character_set, Map<uint32, FontGlyph> glyphs, Texture2D *texture_atlas);
    protected:
        void OnDestroy() override;
    private:
        Font() = default;
        Font(uint32 size, FontCharacterSet character_set, Map<uint32, FontGlyph> glyphs, Texture2D *texture_atlas);

        static Font *Create();
    private:
        uint32 m_size;
        FontCharacterSet m_character_set;
        Map<uint32, FontGlyph> m_glyphs;
        Texture2D *m_texture_atlas;
    };

    class IFontLoader {
    public:
        virtual ~IFontLoader() = default;

        virtual Font *LoadFont(const String &path, uint32 size, FontCharacterSet character_set) = 0;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    };

    class FontLoader final {
    public:
        static Font *LoadFont(const String &path, uint32 size, FontCharacterSet character_set) { return s_font_loader->LoadFont(path, size, character_set); }
    private:
        FontLoader() = delete;
        ~FontLoader() = delete;

        static void Initialize();
        static void Shutdown();
    private:
        inline static IFontLoader *s_font_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}