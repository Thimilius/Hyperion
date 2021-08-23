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
        inline const Vector<byte> &GetPixels() const { return m_pixels; }
    public:
        static Image *Create();
        static Image *Create(uint32 width, uint32 height, uint32 channels, Vector<byte> &&pixels);
    private:
        Image() = default;
        Image(uint32 width, uint32 height, uint32 channels, Vector<byte> &&pixels);
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_channels;

        Vector<byte> m_pixels;
    };

}