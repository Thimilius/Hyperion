#pragma once

#include "hyperion/common.hpp"
#include "hyperion/assets/asset.hpp"

namespace Hyperion::Rendering {

    enum class TextureDimension {
        Texture2D,
        TextureCubemap
    };

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

        virtual TextureDimension GetDimension() const = 0;

        inline TextureFormat GetFormat() const { return m_format; }
        inline u32 GetMipmapCount() const { return m_mipmap_count; }

        virtual void SetWrapMode(TextureWrapMode wrap_mode) = 0;
        inline TextureWrapMode GetWrapMode() const { return m_wrap_mode; }

        virtual void SetFilter(TextureFilter filter) = 0;
        inline TextureFilter GetFilter() const { return m_filter; }

        virtual void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) = 0;
        inline TextureAnisotropicFilter GetAnisotropicFilter() const { return m_anisotropic_filter; }

        virtual u32 GetID() const = 0;
        
        virtual void GenerateMipmaps() = 0;
    protected:
        static u32 GetBytesPerPixel(TextureFormat format);
        static u32 CalculateMipmapCount(u32 width, u32 height);
    };

}