#pragma once

#include "hyperion/rendering/render_texture.hpp"

namespace Hyperion::Rendering {

    class COpenGLRenderTexture : public CRenderTexture {
    private:
        u32 m_framebuffer_id;
        u32 m_color_attachment_id;
        u32 m_depth_attachment_id;
    public:
        COpenGLRenderTexture(u32 width, u32 height, ERenderTextureFormat format);
        ~COpenGLRenderTexture();

        virtual void Bind(ERenderTextureTarget target = ERenderTextureTarget::DrawAndRead) const override;
        virtual void Unbind() const override;

        virtual void Resize(u32 width, u32 height) override;
    };

}