#include "hyppch.hpp"
#include "opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    COpenGLTexture2D::COpenGLTexture2D(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter)
        : COpenGLTexture2D(width, height, format, wrap_mode, filter, anisotropic_filter, nullptr) { }

    COpenGLTexture2D::COpenGLTexture2D(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter, const u8 *pixels) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_wrap_mode = wrap_mode;
        m_filter = filter;
        m_anisotropic_filter = anisotropic_filter;

        CreateTexture(pixels);
    }

    COpenGLTexture2D::COpenGLTexture2D(const TString &path, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter) {
        m_wrap_mode = wrap_mode;
        m_filter = filter;
        m_anisotropic_filter = anisotropic_filter;

        stbi_set_flip_vertically_on_load(true);

        int width;
        int height;
        int channels;
        u8 *pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (pixels == nullptr) {
            HYP_LOG_ERROR("OpenGL", "Failed to load texture from path: {}!", path);
        } else {
            ETextureFormat format = ETextureFormat::None;
            switch (channels) {
                case 3: format = ETextureFormat::RGB; break;
                case 4: format = ETextureFormat::RGBA; break;
            }

            m_width = width;
            m_height = height;
            m_format = format;

            CreateTexture(pixels);
        }

        stbi_image_free(pixels);
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
        glBindTexture(GL_TEXTURE_2D, 0);
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

    void COpenGLTexture2D::SetAnisotropicFilter(ETextureAnisotropicFilter anisotropic_filter) {
        float amount = 1.0f;
        switch (anisotropic_filter) {
            case Hyperion::Rendering::ETextureAnisotropicFilter::None: amount = 1.0f; break;
            case Hyperion::Rendering::ETextureAnisotropicFilter::Times2: amount = 2.0f; break;
            case Hyperion::Rendering::ETextureAnisotropicFilter::Times4: amount = 4.0f; break;
            case Hyperion::Rendering::ETextureAnisotropicFilter::Times8: amount = 8.0f; break;
            case Hyperion::Rendering::ETextureAnisotropicFilter::Times16: amount = 16.0f; break;
            default: amount = 1.0f; break;
        }
        glTextureParameterf(m_texture_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, amount);
    }

    void COpenGLTexture2D::CreateTexture(const u8 *pixels) {
        glGenTextures(1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);

        SetWrapMode(m_wrap_mode);
        SetFilter(m_filter);
        SetAnisotropicFilter(m_anisotropic_filter);

        glTexImage2D(GL_TEXTURE_2D, 0, GetGLFormat(), m_width, m_height, 0, GetGLFormat(), GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
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