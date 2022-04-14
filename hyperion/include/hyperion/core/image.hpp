//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Image {
  public:
    inline uint32 GetWidth() const { return m_width; }
    inline uint32 GetHeight() const { return m_height; }
    inline uint32 GetChannels() const { return m_channels; }
    inline const Array<byte> &GetPixels() const { return m_pixels; }
  public:
    static Image *Create(uint32 width, uint32 height, uint32 channels, Array<byte> &&pixels);
  private:
    Image(uint32 width, uint32 height, uint32 channels, Array<byte> &&pixels);
  private:
    uint32 m_width;
    uint32 m_height;
    uint32 m_channels;

    Array<byte> m_pixels;
  };

}
