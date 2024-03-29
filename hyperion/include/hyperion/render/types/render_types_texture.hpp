//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class TextureDimension {
    Texture2D,
    TextureCubemap,
    RenderTexture
  };

  enum class TextureFormat {
    None,
    
    RGBA32,
    RGB24,
    R8
  };

  enum class TextureWrapMode {
    Clamp,
    Border,
    Repeat,
    MirroredRepeat
  };

  enum class TextureFilter {
    Point,
    Bilinear,
    Trilinear
  };

  enum class TextureAnisotropicFilter {
    None,
    Times2,
    Times4,
    Times8,
    Times16
  };

  struct TextureAttributes {
    TextureWrapMode wrap_mode = TextureWrapMode::Clamp;
    TextureFilter filter = TextureFilter::Bilinear;
    TextureAnisotropicFilter anisotropic_filter = TextureAnisotropicFilter::None;
    bool8 use_mipmaps = true;
    Color border_color = Color::Black();
  };

  struct Texture2DParameters {
    TextureFormat format = TextureFormat::RGB24;
    TextureAttributes attributes = TextureAttributes();

    uint32 width = 0;
    uint32 height = 0;
  };

  using TexturePixelData = Array<byte>;

  enum class RenderTextureFormat {
    RGBA8,
    R32UInt,
    RG32UInt,
    Depth24,
    Depth24Stencil8,
  };

  struct RenderTextureAttachment {
    RenderTextureFormat format = RenderTextureFormat::RGBA8;
    TextureAttributes attributes;
    bool8 readable = true;
  };

  struct RenderTextureParameters {
    Array<Rendering::RenderTextureAttachment> attachments;

    uint32 width = 0;
    uint32 height = 0;
  };

}
