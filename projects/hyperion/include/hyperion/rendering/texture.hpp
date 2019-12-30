#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"

namespace Hyperion::Rendering {

    enum class TextureFormat {
        RGB,
        RGBA
    };

    enum class TextureWrapMode {
        Clamp,
        Border,
        Repeat,
        MirroredRepeat
    };

    enum class TextureFilter {
        Point,
        Bilinear,
        Trilinear
    };

    enum class TextureAnisotropicFilter {
        None,
        Times2,
        Times4,
        Times8,
        Times16
    };

    class Texture : public Asset {
    protected:
        TextureFormat m_format;
        TextureWrapMode m_wrap_mode;
        TextureFilter m_filter;
        TextureAnisotropicFilter m_anisotropic_filter;
        u32 m_mipmap_count;
    public:
        virtual ~Texture() = default;

        AssetType GetType() const override { return AssetType::Texture; }

        virtual void Bind(u32 slot = 0) const = 0;
        virtual void Unbind(u32 slot = 0) const = 0;

        virtual u32 GetID() const = 0;

        inline TextureFormat GetFormat() const { return m_format; }
        inline u32 GetMipmapCount() const { return m_mipmap_count; }

        virtual void SetWrapMode(TextureWrapMode wrap_mode) = 0;
        inline TextureWrapMode GetWrapMode() const { return m_wrap_mode; }

        virtual void SetFilter(TextureFilter filter) = 0;
        inline TextureFilter GetFilter() const { return m_filter; }

        virtual void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) = 0;
        inline TextureAnisotropicFilter GetAnisotropicFilter() const { return m_anisotropic_filter; }

        static u32 GetBytesPerPixel(TextureFormat format);
        static u32 CalculateMipmapCount(u32 width, u32 height);
    };

    class Texture2D : public Texture {
    protected:
        u32 m_width;
        u32 m_height;
    public:
        virtual ~Texture2D() = default;
        
        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        virtual void Resize(u32 width, u32 height) = 0;
        virtual void Resize(u32 width, u32 height, TextureFormat format) = 0;

        virtual void SetPixels(const u8 *pixels) = 0;
        virtual u8 *GetPixels() = 0;

        static Ref<Texture2D> Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter);
        static Ref<Texture2D> Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels);
    };

}