#include "hyppch.hpp"

#include "hyperion/core/image.hpp"

namespace Hyperion {

    Image::Image(u32 width, u32 height, u32 channels, u8 *pixels)
        : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels) { }

    Image::~Image() {
        delete[] m_pixels;
    }

    Ref<Image> Image::Create(u32 width, u32 height, u32 channels, u8 *pixels) {
        return Ref<Image>(new Image(width, height, channels, pixels));
    }

}