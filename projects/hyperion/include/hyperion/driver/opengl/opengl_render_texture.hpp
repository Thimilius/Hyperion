#pragma once

#include "hyperion/rendering/render_texture.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderTexture : public RenderTexture {
    private:
        u32 m_framebuffer_id;
        u32 m_color_attachment_id;
        u32 m_depth_attachment_id;
    public:
        OpenGLRenderTexture(u32 width, u32 height, RenderTextureFormat format);
        ~OpenGLRenderTexture() override;

        virtual u32 GetID() const { return m_framebuffer_id; }

        void BindTexture(u32 slot = 0) override;

        virtual void Resize(u32 width, u32 height) override;
    };

}