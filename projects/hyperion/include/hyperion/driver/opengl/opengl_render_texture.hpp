#pragma once

#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/texture_2d.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderTexture : public RenderTexture {
    private:
        u32 m_framebuffer_id;
        u32 m_depth_attachment_id;

        Ref<Texture2D> m_color_attachment;
    public:
        OpenGLRenderTexture(u32 width, u32 height, RenderTextureFormat format);
        ~OpenGLRenderTexture() override;

        u32 GetID() const override { return m_framebuffer_id; }

        void SetWrapMode(TextureWrapMode wrap_mode) override;
        void SetFilter(TextureFilter filter) override;
        void SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) override;

        void BindTexture(u32 slot = 0) override;

        virtual void Resize(u32 width, u32 height) override;
    };

}