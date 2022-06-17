//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/font_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Font final : public Asset {
    HYP_REFLECT(Asset)
  public:
    AssetType GetAssetType() const override { return AssetType::Font; }

    inline uint32 GetSize() const { return m_size; }
    inline float32 GetBaselineOffset() const { return m_baseline_offset; }
    inline Texture2D *GetTexture() const { return m_font_atlas->GetTexture(); }
    const FontGlyph &GetGlyph(uint32 codepoint) const;
    inline bool8 HasCodepoint(uint32 codepoint) const { return m_font_atlas->HasElement(codepoint); }
    const FontAtlasElement &GetElement(uint32 codepoint) const;
    inline const SpecialFontGlyphs &GetSpecialGlyphs() const { return m_special_glyphs; }
  private:
    Font(AssetMetadata metadata) : Asset(std::move(metadata)) { }
    Font(AssetMetadata metadata, uint32 size, float32 baseline_offset, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs);
  private:
    uint32 m_size = 0;
    float32 m_baseline_offset = 0;
    FontCharacterSet m_character_set = FontCharacterSet::None;
    FontAtlas *m_font_atlas = nullptr;
    SpecialFontGlyphs m_special_glyphs = { };
  private:
    friend class Hyperion::AssetManager;
  };

}
