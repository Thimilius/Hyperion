#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture_cubemap.hpp"

#include "glad/glad.h"

namespace Hyperion::Rendering {

    OpenGLTextureCubemap::OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format, TextureParameters parameters)
        : OpenGLTextureCubemap(width, height, format, parameters, { }) { }

    OpenGLTextureCubemap::OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const Map<TextureCubemapFace, const u8 *> &pixels) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_parameters = parameters;
        m_mipmap_count = m_parameters.use_mipmaps ? CalculateMipmapCount(width, height) : 0;

        CreateTexture(pixels);
    }

    OpenGLTextureCubemap::~OpenGLTextureCubemap() {
        glDeleteTextures(1, &m_texture_id);
    }

    void OpenGLTextureCubemap::Bind(u32 slot) const {
        glBindTextureUnit(slot, m_texture_id);
    }

    void OpenGLTextureCubemap::Unbind(u32 slot) const {
        glBindTextureUnit(slot, 0);
    }

    void OpenGLTextureCubemap::GenerateMipmaps() {
        glGenerateTextureMipmap(m_texture_id);
    }

    void OpenGLTextureCubemap::SetWrapMode(TextureWrapMode wrap_mode) {
        m_parameters.wrap_mode = wrap_mode;

        u32 wrap = GetGLWrapMode(wrap_mode);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_R, wrap);
    }

    void OpenGLTextureCubemap::SetFilter(TextureFilter filter) {
        m_parameters.filter = filter;

        u32 min_filter = GetGLMinFilter(filter);
        u32 mag_filter = GetGLMaxFilter(filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MIN_FILTER, min_filter);
        glTextureParameteri(m_texture_id, GL_TEXTURE_MAG_FILTER, mag_filter);
    }

    void OpenGLTextureCubemap::SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        m_parameters.anisotropic_filter = anisotropic_filter;

        glTextureParameterf(m_texture_id, GL_TEXTURE_MAX_ANISOTROPY, GetGLAnisotropicFilter(anisotropic_filter));
    }

    void OpenGLTextureCubemap::SetPixels(TextureCubemapFace face, const u8 *pixels, bool generate_mipmaps) {
        u32 face_offset = 0;
        switch (face) {
            case TextureCubemapFace::PositiveX: face_offset = 0; break;
            case TextureCubemapFace::NegativeX: face_offset = 1; break;
            case TextureCubemapFace::PositiveY: face_offset = 2; break;
            case TextureCubemapFace::NegativeY: face_offset = 3; break;
            case TextureCubemapFace::PositiveZ: face_offset = 4; break;
            case TextureCubemapFace::NegativeZ: face_offset = 5; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        SetUnpackAlignmentForFormat(m_format);
        glTextureSubImage3D(m_texture_id, 0, 0, 0, face_offset, m_width, m_height, 1, GetGLFormat(m_format), GetGLFormatType(m_format), pixels);

        if (generate_mipmaps) {
            GenerateMipmaps();
        }
    }

    void OpenGLTextureCubemap::CreateTexture(const Map<TextureCubemapFace, const u8 *> &pixels) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture_id);

        bool valid_pixels = pixels.size() == 0 || pixels.size() == 6;
        if (!valid_pixels) {
            HYP_ASSERT_MESSAGE(false, "Can't create cube map from less than 6 faces!");
        }

        SetFilter(m_parameters.filter);
        SetWrapMode(m_parameters.wrap_mode);
        SetAnisotropicFilter(m_parameters.anisotropic_filter);

        glTextureStorage2D(m_texture_id, m_parameters.use_mipmaps ? m_mipmap_count : 1, GetGLInternalFormat(m_format), m_width, m_height);

        if (valid_pixels) {
            for (auto [face, pixel_data] : pixels) {
                SetPixels(face, pixel_data, false);
            }

            GenerateMipmaps();
        }
    }

}