//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/texture.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/texture_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Texture2D::Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters) : Texture(info) {
    m_width = parameters.width;
    m_height = parameters.height;
    m_format = parameters.format;
    m_attributes = parameters.attributes;
    m_mipmap_count = TextureUtilities::CalculateMipmapCount(m_width, m_height);
  }

  //--------------------------------------------------------------
  Texture2D::Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters, const Rendering::TexturePixelData &pixels) : Texture2D(info, parameters) {
    SetPixelsInternal(pixels);
  }

  //--------------------------------------------------------------
  const Rendering::TexturePixelData &Texture2D::GetPixels() const {
    ValidateDataAccess();
    return m_pixels;
  }

  //--------------------------------------------------------------
  void Texture2D::SetPixels(const Rendering::TexturePixelData &pixels) {
    if (!ValidateDataAccess()) {
      return;
    }

    SetPixelsInternal(pixels);
  }

  //--------------------------------------------------------------
  void Texture2D::SetPixelsInternal(const Rendering::TexturePixelData &data) {
    m_pixels = data;

    SetDirty();
  }

  //--------------------------------------------------------------
  RenderTexture::RenderTexture(AssetInfo info, const Rendering::RenderTextureParameters &parameters) : Texture(info) {
    m_width = parameters.width;
    m_height = parameters.height;
    m_attachments = parameters.attachments;
    m_mipmap_count = TextureUtilities::CalculateMipmapCount(m_width, m_height);

    SetDirty();
  }

  //--------------------------------------------------------------
  Rendering::RenderTargetId RenderTexture::GetRenderTargetId() const {
    Rendering::RenderTargetId id = { GetAssetInfo().id };
    return id;
  }

  //--------------------------------------------------------------
  void RenderTexture::Resize(uint32 width, uint32 height) {
    if (m_width != width || m_height != height) {
      m_width = width;
      m_height = height;

      SetDirty();
    }
  }

}