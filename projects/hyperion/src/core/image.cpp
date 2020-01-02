#include "hyppch.hpp"

#include "hyperion/core/image.hpp"

namespace Hyperion {

    Image::Image(u32 width, u32 height, u32 channels, u8 *pixels, bool free)
        : m_width(width), m_height(height), m_channels(channels), m_pixels(pixels), m_free(free) { }

    Image::~Image() {
        Clear();
    }

    Ref<Image> Image::Create(u32 width, u32 height, u32 channels, u8 *pixels, bool free) {
        return Ref<Image>(new Image(width, height, channels, pixels, free));
    }

    void Image::Clear() {
        if (m_free) {
            std::free(m_pixels);
        } else {
            delete[] m_pixels;
        }
    }

}