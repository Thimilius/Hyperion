#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_texture.hpp"

namespace Hyperion::Rendering {

    OpenGLRenderTexture::OpenGLRenderTexture(u32 width, u32 height, RenderTextureFormat format) {
        m_format = format;
        
        Resize(width, height);
    }
    
    OpenGLRenderTexture::~OpenGLRenderTexture() {
        glDeleteFramebuffers(1, &m_framebuffer_id);
    }

    void OpenGLRenderTexture::BindTexture(u32 slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_color_attachment_id);
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
            glDeleteTextures(1, &m_color_attachment_id);
            glDeleteRenderbuffers(1, &m_depth_attachment_id);
        }

        glCreateFramebuffers(1, &m_framebuffer_id);

        // Add color attachment
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment_id);
            glBindTexture(GL_TEXTURE_2D, m_color_attachment_id);

            switch (m_format) {
                case Hyperion::Rendering::RenderTextureFormat::RGBA8:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
                    break;
                case Hyperion::Rendering::RenderTextureFormat::RGBA16F:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
                    break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }

            glTextureParameteri(m_color_attachment_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_color_attachment_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glNamedFramebufferTexture(m_framebuffer_id, GL_COLOR_ATTACHMENT0, m_color_attachment_id, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}