#include "hyppch.hpp"

#include "hyperion/core/image.hpp"

namespace Hyperion {

    Image::Image(uint32 width, uint32 height, uint32 channels, uint8 *pixels)
        : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels) { }

    Image::~Image() {
        delete[] m_pixels;
    }

    Image *Image::Create(uint32 width, uint32 height, uint32 channels, uint8 *pixels) {
        return new Image(width, height, channels, pixels);
    }

}