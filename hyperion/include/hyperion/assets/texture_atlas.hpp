//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/assets/texture_atlas_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class TextureAtlasBase : public Asset {
    HYP_REFLECT(Asset)
  public:
    inline AssetType GetAssetType() const override { return AssetType::TextureAtlas; }
  protected:
    TextureAtlasBase(AssetInfo info) : Asset(info) { }
  };

  template<typename K, typename V>
  class TextureAtlas : public TextureAtlasBase {
    HYP_REFLECT(TextureAtlasBase)
  public:
    inline Texture2D *GetTexture() const { return m_texture; }

    inline bool8 HasElement(const K &key) const { return m_elements.Contains(key); }

    inline const TextureAtlasElement<V> &GetElement(const K &key) const {
      // TODO: Handle not finding an element.
      auto it = m_elements.Find(key);
      const TextureAtlasElement<V> &element = it->second;
      return element;
    }
  private:
    TextureAtlas(AssetInfo info) : TextureAtlasBase(info) { }

    TextureAtlas(AssetInfo info, Texture2D *texture, const Map<K, TextureAtlasElement<V>> &elements) : TextureAtlas(info) {
      m_texture = texture;
      m_elements = elements;
    }
  private:
    Texture2D *m_texture;
    Map<K, TextureAtlasElement<V>> m_elements;
  private:
    friend class Hyperion::AssetManager;
  };

}
