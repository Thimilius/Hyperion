//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/font_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct TextSize {
    float32 width;
    float32 height;
    float32 baseline_offset;
  };

  class Font final : public Asset {
  public:
    AssetType GetAssetType() const override { return AssetType::Font; }

    inline uint32 GetSize() const { return m_size; }
    inline Texture2D *GetTexture() const { return m_font_atlas->GetTexture(); }
    const FontGlyph &GetGlyph(uint32 codepoint) const;
    inline bool8 HasCodepoint(uint32 codepoint) const { return m_font_atlas->HasElement(codepoint); }
    const FontAtlasElement &GetElement(uint32 codepoint) const;
    inline const SpecialFontGlyphs &GetSpecialGlyphs() const { return m_special_glyphs; }

    TextSize GetTextSize(const Array<uint32> &codepoints, uint32 codepoint_offset, float32 scale, bool8 line_only) const;
  private:
    Font(AssetInfo info) : Asset(info) { }
    Font(AssetInfo info, uint32 size, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs);
  private:
    uint32 m_size;
    FontCharacterSet m_character_set;
    FontAtlas *m_font_atlas;
    SpecialFontGlyphs m_special_glyphs;
  private:
    friend class Hyperion::AssetManager;
  };

}