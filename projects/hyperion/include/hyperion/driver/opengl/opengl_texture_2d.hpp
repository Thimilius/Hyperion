#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    class OpenGLTexture2D : public Texture2D {
    private:
        u32 m_texture_id;
    public:
        OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter);
        OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels);
        OpenGLTexture2D(const String &path, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter);
        ~OpenGLTexture2D() override;

        void Bind(u32 slot = 0) const override;
        void Unbind(u32 slot = 0) const override;

        u32 GetID() const override { return m_texture_id; }

        void SetWrapMode(TextureWrapMode wrap_mode) override;
        void SetFilter(TextureFilter filter) override;
        void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) override;

        void Resize(u32 width, u32 height) override;
        void Resize(u32 width, u32 height, TextureFormat format) override;

        void SetPixels(const u8 *pixels) override;
        void *GetPixels() override;
    private:
        void CreateTexture(const u8 *pixels);

        static u32 GetGLFormat(TextureFormat format);
        static u32 GetGLWrapMode(TextureWrapMode wrap_mode);
    };

}