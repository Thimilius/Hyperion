//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/image.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Image::Image(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels)
        : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels) { }

    //--------------------------------------------------------------
    Image *Image::Create() {
        return new Image();
    }

    //--------------------------------------------------------------
    Image *Image::Create(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels) {
        return new Image(width, height, channels, std::move(pixels));
    }

}