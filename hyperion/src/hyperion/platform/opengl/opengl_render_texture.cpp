#include "hyppch.hpp"

#include "opengl_render_texture.hpp"

namespace Hyperion::Rendering {

    COpenGLRenderTexture::COpenGLRenderTexture(u32 width, u32 height, ERenderTextureFormat format) {
        m_width = width;
        m_height = height;
        m_format = format;

        glGenFramebuffers(1, &m_framebuffer_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);

        // Add color attachment
        {
            glGenTextures(1, &m_color_attachment_id);
            glBindTexture(GL_TEXTURE_2D, m_color_attachment_id);
            
            switch (format) {
                case Hyperion::Rendering::ERenderTextureFormat::RGBA8:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                    break;
                case Hyperion::Rendering::ERenderTextureFormat::RGBA16F:
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, NULL);
                    break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment_id, 0);
        }
        

        // Add depth-stencil attachment
        {
            glGenRenderbuffers(1, &m_depth_attachment_id);
            glBindRenderbuffer(GL_RENDERBUFFER, m_depth_attachment_id);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depth_attachment_id);
        }

        // Check for completion
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            HYP_CORE_ERROR("[OpenGL] - Failed to generate framebuffer!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    COpenGLRenderTexture::~COpenGLRenderTexture() {
        glDeleteFramebuffers(1, &m_framebuffer_id);
    }

    void COpenGLRenderTexture::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);
        glViewport(0, 0, m_width, m_height);
    }

    void COpenGLRenderTexture::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}