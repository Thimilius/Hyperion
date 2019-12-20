#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    OpenGLTexture2D::OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter)
        : OpenGLTexture2D(width, height, format, wrap_mode, filter, anisotropic_filter, nullptr) { }

    OpenGLTexture2D::OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_wrap_mode = wrap_mode;
        m_filter = filter;
        m_anisotropic_filter = anisotropic_filter;

        CreateTexture(pixels);
    }

    OpenGLTexture2D::OpenGLTexture2D(const String &path, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter) {
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
            TextureFormat format = TextureFormat::None;
            switch (channels) {
                case 3: format = TextureFormat::RGB; break;
                case 4: format = TextureFormat::RGBA; break;
            }

            m_width = width;
            m_height = height;
            m_format = format;

            CreateTexture(pixels);
        }

        stbi_image_free(pixels);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_texture_id);
    }

    void OpenGLTexture2D::Bind(u32 slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
    }

    void OpenGLTexture2D::Unbind(u32 slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGLTexture2D::SetWrapMode(TextureWrapMode wrap_mode) {
        m_wrap_mode = wrap_mode;
        u32 wrap = GetGLWrapMode(wrap_mode);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, wrap);
    }

    void OpenGLTexture2D::SetFilter(TextureFilter filter) {
        u32 min_filter;
        u32 mag_filter;
        switch (filter) {
            case TextureFilter::Point:
                min_filter = GL_NEAREST_MIPMAP_NEAREST;
                mag_filter = GL_NEAREST;
                break;
            case TextureFilter::Bilinear:
                min_filter = GL_LINEAR_MIPMAP_NEAREST;
                mag_filter = GL_LINEAR;
                break;
            case TextureFilter::Trilinear:
                min_filter = GL_LINEAR_MIPMAP_LINEAR;
                mag_filter = GL_LINEAR;
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        m_filter = filter;
        glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    void OpenGLTexture2D::SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        float amount = 1.0f;
        switch (anisotropic_filter) {
            case Hyperion::Rendering::TextureAnisotropicFilter::None: amount = 1.0f; break;
            case Hyperion::Rendering::TextureAnisotropicFilter::Times2: amount = 2.0f; break;
            case Hyperion::Rendering::TextureAnisotropicFilter::Times4: amount = 4.0f; break;
            case Hyperion::Rendering::TextureAnisotropicFilter::Times8: amount = 8.0f; break;
            case Hyperion::Rendering::TextureAnisotropicFilter::Times16: amount = 16.0f; break;
            default: amount = 1.0f; break;
        }
        glTextureParameterf(m_texture_id, GL_TEXTURE_MAX_ANISOTROPY, amount);
    }

    void OpenGLTexture2D::SetPixels(const void *pixels) {
        glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, GetGLFormat(m_format), GL_UNSIGNED_BYTE, pixels);
    }

    void *OpenGLTexture2D::GetPixels() {
        // FIXME: This is a very expensive allocation
        u32 size = m_width * m_height * GetBytesPerPixel(m_format);
        char *pixels = new char[size];

        // TODO: Provide ability to get pixels from other mipmaps
        glGetTextureImage(m_texture_id, 0, GetGLFormat(m_format), GL_UNSIGNED_BYTE, size, pixels);

        return pixels;
    }

    void OpenGLTexture2D::CreateTexture(const u8 *pixels) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);

        SetWrapMode(m_wrap_mode);
        SetFilter(m_filter);
        SetAnisotropicFilter(m_anisotropic_filter);

        auto format = GetGLFormat(m_format);
        glTextureImage2DEXT(m_texture_id, GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateTextureMipmap(m_texture_id);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    u32 OpenGLTexture2D::GetGLFormat(TextureFormat format) {
        switch (format) {
            case Hyperion::Rendering::TextureFormat::RGB: return GL_RGB;
            case Hyperion::Rendering::TextureFormat::RGBA: return GL_RGBA;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture2D::GetGLWrapMode(TextureWrapMode wrap_mode) {
        switch (wrap_mode) {
            case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::Repeat: return GL_REPEAT;
            case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}