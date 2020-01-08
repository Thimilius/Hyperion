#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_texture.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    OpenGLRenderTexture::OpenGLRenderTexture(u32 width, u32 height, RenderTextureFormat format) {
        m_render_format = format;
        m_format = GetTextureFormat(m_render_format);
        m_wrap_mode = TextureWrapMode::Clamp;
        m_filter = TextureFilter::Point;
        m_anisotropic_filter = TextureAnisotropicFilter::None;
        m_mipmap_count = 0;

        m_framebuffer_id = 0;
        Resize(width, height);
    }
    
    OpenGLRenderTexture::~OpenGLRenderTexture() {
        glDeleteFramebuffers(1, &m_framebuffer_id);
    }

    void OpenGLRenderTexture::GenerateMipmaps() {
        // TODO: Implement mipmaps for render textures
    }

    void OpenGLRenderTexture::SetWrapMode(TextureWrapMode wrap_mode) {
        m_wrap_mode = wrap_mode;
        m_color_attachment->SetWrapMode(wrap_mode);
    }

    void OpenGLRenderTexture::SetFilter(TextureFilter filter) {
        m_filter = filter;
        m_color_attachment->SetFilter(filter);
    }

    void OpenGLRenderTexture::SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        m_anisotropic_filter = anisotropic_filter;
        m_color_attachment->SetAnisotropicFilter(anisotropic_filter);
    }

    void OpenGLRenderTexture::BindTexture(u32 slot) {
        m_color_attachment->Bind(slot);
    }

    void OpenGLRenderTexture::Resize(u32 width, u32 height) {
        if (m_width == width && m_height == height) {
            return;
        }

        m_width = width;
        m_height = height;

        // Delete old resources if present
        if (m_framebuffer_id != 0) {
            glDeleteFramebuffers(1, &m_framebuffer_id);
            glDeleteRenderbuffers(1, &m_depth_attachment_id);
        } else {
            // Create initial texture attachment
            m_color_attachment = Texture2D::Create(width, height, m_format, m_wrap_mode, m_filter, m_anisotropic_filter);
        }

        glCreateFramebuffers(1, &m_framebuffer_id);

        // Add color attachment
        {
            m_color_attachment->Resize(width, height);
            glNamedFramebufferTexture(m_framebuffer_id, GL_COLOR_ATTACHMENT0, m_color_attachment->GetID(), 0);
        }

        // Add depth-stencil attachment
        {
            glCreateRenderbuffers(1, &m_depth_attachment_id);
            glNamedRenderbufferStorage(m_depth_attachment_id, GL_DEPTH24_STENCIL8, width, height);
            glNamedFramebufferRenderbuffer(m_framebuffer_id, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_id);
        }

        // Check for completion
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            HYP_LOG_ERROR("OpenGL", "Failed to generate framebuffer!");
        }
    }

}