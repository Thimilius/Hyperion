#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Image {
    public:
        ~Image();

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        inline uint32 GetChannels() const { return m_channels; }
        inline uint8 *GetPixels() const { return m_pixels; }

        inline bool IsEmpty() const { return m_width == 0 && m_height == 0 && m_channels == 0 && m_pixels == nullptr; }

        static Image *Create(uint32 width, uint32 height, uint32 channels, uint8 *pixels);
    private:
        Image(uint32 width, uint32 height, uint32 channels, uint8 *pixels);
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_channels;
        uint8 *m_pixels;
    };

}