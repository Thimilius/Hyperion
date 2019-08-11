#include "hyppch.hpp"
#include "opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    COpenGLTexture2D::COpenGLTexture2D(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter)
        : COpenGLTexture2D(width, height, mode, wrap_mode, filter, nullptr) { }

    COpenGLTexture2D::COpenGLTexture2D(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter, const void *pixels) {
        m_width = width;
        m_height = height;
        m_format = mode;
        m_wrap_mode = wrap_mode;
        m_filter = filter;

        glGenTextures(1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);

        SetFilter(filter);
        SetWrapMode(wrap_mode);

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GetGLFormat(), GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glGenTextures(1, 0);
    }

    COpenGLTexture2D::~COpenGLTexture2D() {
        glDeleteTextures(1, &m_texture_id);
    }

    void COpenGLTexture2D::Bind(u32 slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
    }

    void COpenGLTexture2D::Unbind(u32 slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
    }

    void COpenGLTexture2D::SetData(const void *pixels) {
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GetGLFormat(), GL_UNSIGNED_BYTE, pixels);
    }

    void COpenGLTexture2D::SetWrapMode(ETextureWrapMode wrap_mode) {
        m_wrap_mode = wrap_mode;
        u32 wrap = GetGLWrapMode();
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, wrap);
    }

    void COpenGLTexture2D::SetFilter(ETextureFilter filter) {
        u32 min_filter;
        u32 mag_filter;
        switch (filter) {
            case ETextureFilter::Point:
                min_filter = GL_NEAREST_MIPMAP_NEAREST;
                mag_filter = GL_NEAREST;
                break;
            case ETextureFilter::Bilinear:
                min_filter = GL_LINEAR_MIPMAP_NEAREST;
                mag_filter = GL_LINEAR;
                break;
            case ETextureFilter::Trilinear:
                min_filter = GL_LINEAR_MIPMAP_LINEAR;
                mag_filter = GL_LINEAR;
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        m_filter = filter;
        glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    u32 COpenGLTexture2D::GetGLFormat() const {
        switch (m_format) {
            case Hyperion::Rendering::ETextureFormat::RGB: return GL_RGB;
            case Hyperion::Rendering::ETextureFormat::RGBA: return GL_RGBA;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLTexture2D::GetGLWrapMode() const {
        switch (m_wrap_mode) {
            case ETextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case ETextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case ETextureWrapMode::Repeat: return GL_REPEAT;
            case ETextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}