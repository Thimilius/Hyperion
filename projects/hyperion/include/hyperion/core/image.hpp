#pragma once

#include "hyperion/core/object/object.hpp"

namespace Hyperion {

    class Image : public Object {
        HYP_REFLECT(Object);
    public:
        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        inline uint32 GetChannels() const { return m_channels; }
        inline const Vector<uint8> &GetPixels() const { return m_pixels; }
    public:
        static Image *Create(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels);
    private:
        Image(uint32 width, uint32 height, uint32 channels, Vector<uint8> &&pixels);
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_channels;

        Vector<uint8> m_pixels;
    };

}