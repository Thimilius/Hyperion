#pragma once

#include "hyperion/rendering/texture_2d.hpp"

namespace Hyperion::Rendering {

    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureParameters parameters);
        OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const u8 *pixels);
        ~OpenGLTexture2D() override;

        void Bind(u32 slot) const override;
        void Unbind(u32 slot) const override;

        u32 GetID() const override { return m_texture_id; }

        void GenerateMipmaps() override;

        void SetWrapMode(TextureWrapMode wrap_mode) override;
        void SetFilter(TextureFilter filter) override;
        void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) override;

        void Resize(u32 width, u32 height) override;
        void Resize(u32 width, u32 height, TextureFormat format) override;

        void SetPixels(const u8 *pixels, bool generate_mipmaps) override;
        u8 *GetPixels() const override;
    private:
        void CreateTexture(const u8 *pixels);
    private:
        u32 m_texture_id;
    };

}