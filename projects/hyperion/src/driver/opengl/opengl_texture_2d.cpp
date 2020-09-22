#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture_2d.hpp"

#include <glad/glad.h>

#include "hyperion/driver/opengl/opengl_texture_utilities.hpp"

namespace Hyperion::Rendering {

    OpenGLTexture2D::OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureParameters parameters)
        : OpenGLTexture2D(width, height, format, parameters, nullptr) { }

    OpenGLTexture2D::OpenGLTexture2D(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const u8 *pixels) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_parameters = parameters;
        m_mipmap_count = parameters.use_mipmaps ? Texture::CalculateMipmapCount(width, height) : 0;

        CreateTexture(pixels);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_texture_id);
    }

    void OpenGLTexture2D::Bind(u32 slot) const {
        glBindTextureUnit(slot, m_texture_id);
    }

    void OpenGLTexture2D::Unbind(u32 slot) const {
        glBindTextureUnit(slot, 0);
    }

    void OpenGLTexture2D::GenerateMipmaps() {
        if (m_parameters.use_mipmaps) {
            glGenerateTextureMipmap(m_texture_id);
        }
    }

    void OpenGLTexture2D::SetWrapMode(TextureWrapMode wrap_mode) {
        m_parameters.wrap_mode = wrap_mode;

        u32 wrap = OpenGLTextureUtilities::GetGLWrapMode(wrap_mode);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, wrap);
    }

    void OpenGLTexture2D::SetFilter(TextureFilter filter) {
        m_parameters.filter = filter;

        u32 min_filter = OpenGLTextureUtilities::GetGLMinFilter(filter);
        u32 mag_filter = OpenGLTextureUtilities::GetGLMaxFilter(filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    void OpenGLTexture2D::SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        m_parameters.anisotropic_filter = anisotropic_filter;

        f32 anisotropic_filter_value = OpenGLTextureUtilities::GetGLAnisotropicFilter(anisotropic_filter);
        glTextureParameterf(m_texture_id, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
    }

    void OpenGLTexture2D::Resize(u32 width, u32 height) {
        Resize(width, height, m_format);
    }

    void OpenGLTexture2D::Resize(u32 width, u32 height, TextureFormat format) {
        // If we get passed no new size or format we can bail out early
        if (width == m_width && height == m_height && format == m_format) {
            return;
        }

        // FIXME: The process of copying old pixel data to the new texture is everything but fast!
        u32 old_texture = m_texture_id;
        u32 old_width = m_width;
        u32 old_height = m_height;

        u8 *pixels = GetPixels();

        m_width = width;
        m_height = height;
        m_format = format;
        m_mipmap_count = Texture::CalculateMipmapCount(width, height);

        CreateTexture(nullptr);

        OpenGLTextureUtilities::SetUnpackAlignmentForFormat(m_format);

        u32 sub_image_width = old_width > width ? width : old_width;
        u32 sub_image_height = old_height > height ? height : old_height;
        u32 format_value = OpenGLTextureUtilities::GetGLFormat(m_format);
        u32 format_type = OpenGLTextureUtilities::GetGLFormatType(m_format);

        glTextureSubImage2D(m_texture_id, 0, 0, 0, sub_image_width, sub_image_height, format_value, format_type, pixels);
        
        GenerateMipmaps();

        delete[] pixels;
        glDeleteTextures(1, &old_texture);
    }

    void OpenGLTexture2D::SetPixels(const u8 *pixels, bool generate_mipmaps) {
        OpenGLTextureUtilities::SetUnpackAlignmentForFormat(m_format);

        u32 format_value = OpenGLTextureUtilities::GetGLFormat(m_format);
        u32 format_type = OpenGLTextureUtilities::GetGLFormatType(m_format);

        glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, format_value, format_type, pixels);

        if (generate_mipmaps) {
            GenerateMipmaps();
        }
    }

    u8 *OpenGLTexture2D::GetPixels() const {
        // NOTE: This is a very expensive allocation
        u32 size = m_width * m_height * GetBytesPerPixel(m_format);
        u8 *pixels = new u8[size];

        u32 format_value = OpenGLTextureUtilities::GetGLFormat(m_format);
        u32 format_type = OpenGLTextureUtilities::GetGLFormatType(m_format);

        // TODO: Provide ability to get pixels from other mipmaps
        glGetTextureImage(m_texture_id, 0, format_value, format_type, size, pixels);

        return pixels;
    }

    void OpenGLTexture2D::CreateTexture(const u8 *pixels) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture_id);

        SetWrapMode(m_parameters.wrap_mode);
        SetFilter(m_parameters.filter);
        SetAnisotropicFilter(m_parameters.anisotropic_filter);

        u32 internal_format = OpenGLTextureUtilities::GetGLInternalFormat(m_format);
        glTextureStorage2D(m_texture_id, m_parameters.use_mipmaps ? m_mipmap_count : 1, internal_format, m_width, m_height);

        if (pixels != nullptr) {
            OpenGLTextureUtilities::SetUnpackAlignmentForFormat(m_format);

            u32 format_value = OpenGLTextureUtilities::GetGLFormat(m_format);
            u32 format_type = OpenGLTextureUtilities::GetGLFormatType(m_format);

            glTextureSubImage2D(m_texture_id, 0, 0, 0, m_width, m_height, format_value, format_type, pixels);

            GenerateMipmaps();
        }
    }

}