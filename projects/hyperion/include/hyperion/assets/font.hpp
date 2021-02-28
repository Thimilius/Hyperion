#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/texture.hpp"
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

        Vec2 size;
        Vec2 bearing;
        uint32 advance;
    };

    struct SpecialFontGlyphs {
        FontGlyph space;
    };

    using FontAtlas = TextureAtlas<uint32, FontGlyph>;
    using FontAtlasElement = TextureAtlasElement<FontGlyph>;
    using FontAtlasMap = Map<uint32, FontAtlasElement>;

    class Font : public Asset {
        HYP_REFLECT(Asset);
    public:
        AssetType GetAssetType() const override { return AssetType::Font; }

        inline uint32 GetSize() const { return m_size; }
        inline Texture2D *GetTexture() const { return m_font_atlas->GetTexture(); }
        const FontGlyph &GetGlyph(uint32 codepoint) const;
        const FontAtlasElement &GetElement(uint32 codepoint) const;
        inline const SpecialFontGlyphs &GetSpecialGlyphs() const { return m_special_glyphs; }

        Vec2 GetTextSize(const String &text, float32 scale) const;
    public:
        static Font *Create(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs);
    protected:
        void OnDestroy() override;
    private:
        Font() = default;
        Font(uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs);
    private:
        static Font *Create();
    private:
        uint32 m_size;
        FontCharacterSet m_character_set;
        FontAtlas *m_font_atlas;
        SpecialFontGlyphs m_special_glyphs;
    };

    class IFontLoader {
    public:
        virtual ~IFontLoader() = default;
    public:
        virtual Font *LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) = 0;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    };

    class FontLoader final {
    public:
        static Font *LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) { return s_font_loader->LoadFont(path, font_size, character_set); }
    private:
        FontLoader() = delete;
        ~FontLoader() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static IFontLoader *s_font_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}