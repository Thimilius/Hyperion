#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Image {
    public:
        ~Image();

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        inline u32 GetChannels() const { return m_channels; }
        inline u8 *GetPixels() const { return m_pixels; }

        inline bool IsEmpty() const { return m_width == 0 && m_height == 0 && m_channels == 0 && m_pixels == nullptr; }

        static Ref<Image> Create(u32 width, u32 height, u32 channels, u8 *pixels);
    private:
        Image(u32 width, u32 height, u32 channels, u8 *pixels);
    private:
        u32 m_width;
        u32 m_height;
        u32 m_channels;
        u8 *m_pixels;
    };

}