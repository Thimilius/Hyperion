#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ETextureFormat {
        None,
        RGB,
        RGBA
    };

    enum class ETextureWrapMode {
        None,
        Clamp,
        Border,
        Repeat,
        MirroredRepeat
    };

    enum class ETextureFilter {
        Point,
        Bilinear,
        Trilinear
    };

    class CTexture {
    protected:
        ETextureFormat m_format;
        ETextureWrapMode m_wrap_mode;
        ETextureFilter m_filter;
    public:
        virtual ~CTexture() = default;

        virtual void Bind(u32 slot = 0) const = 0;
        virtual void Unbind(u32 slot = 0) const = 0;

        virtual void SetData(const void *pixels) = 0;

        inline ETextureFormat GetFormat() const { return m_format; }

        virtual void SetWrapMode(ETextureWrapMode wrap_mode) = 0;
        inline ETextureWrapMode GetWrapMode() const { return m_wrap_mode; }

        virtual void SetFilter(ETextureFilter filter) = 0;
        inline ETextureFilter GetFilter() const { return m_filter; }
    };

    class CTexture2D : public CTexture {
    protected:
        u32 m_width;
        u32 m_height;
    public:
        virtual ~CTexture2D() = default;
        
        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        static CTexture2D *Create(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter);
    };

}