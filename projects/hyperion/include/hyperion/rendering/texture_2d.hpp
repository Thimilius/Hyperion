#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    class Texture2D : public Texture {
    protected:
        u32 m_width;
        u32 m_height;
    public:
        virtual ~Texture2D() = default;

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        virtual void Bind(u32 slot = 0) const = 0;
        virtual void Unbind(u32 slot = 0) const = 0;

        virtual void Resize(u32 width, u32 height) = 0;
        virtual void Resize(u32 width, u32 height, TextureFormat format) = 0;

        virtual void SetPixels(const u8 *pixels) = 0;
        virtual u8 *GetPixels() = 0;

        static Ref<Texture2D> Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter);
        static Ref<Texture2D> Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels);
    };

}