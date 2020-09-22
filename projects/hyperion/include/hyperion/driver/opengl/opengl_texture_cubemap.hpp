#pragma once

#include "hyperion/rendering/texture_cubemap.hpp"

namespace Hyperion::Rendering {

    class OpenGLTextureCubemap : public TextureCubemap {
    public:
        OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format, TextureParameters parameters);
        OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const Map<TextureCubemapFace, const u8 *> &pixels);
        ~OpenGLTextureCubemap() override;

        void Bind(u32 slot = 0) const;
        void Unbind(u32 slot = 0) const;

        u32 GetID() const override { return m_texture_id; }

        void GenerateMipmaps() override;

        void SetWrapMode(TextureWrapMode wrap_mode) override;
        void SetFilter(TextureFilter filter) override;
        void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) override;

        void SetPixels(TextureCubemapFace face, const u8 *pixels, bool generate_mipmaps) override;
    private:
        void CreateTexture(const Map<TextureCubemapFace, const u8 *> &pixels);
    private:
        u32 m_texture_id;
    };

}