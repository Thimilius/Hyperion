#include "hyppch.hpp"

#include "hyperion/core/image.hpp"

namespace Hyperion {

    Image::Image(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels)
        : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels) { }

    Image *Image::Create(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels) {
        return new Image(width, height, channels, std::move(pixels));
    }

}