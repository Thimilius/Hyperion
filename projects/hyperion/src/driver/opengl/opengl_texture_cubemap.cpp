#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture_cubemap.hpp"

#include "glad/glad.h"

// TODO: Remove code duplication from texture 2D

namespace Hyperion::Rendering {

    OpenGLTextureCubemap::OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format) 
        : OpenGLTextureCubemap(width, height, format, { }) { }

    OpenGLTextureCubemap::OpenGLTextureCubemap(u32 width, u32 height, TextureFormat format, const Map<TextureCubemapFace, const u8 *> &pixels) {
        m_width = width;
        m_height = height;
        m_format = format;
        m_wrap_mode = TextureWrapMode::Clamp;
        m_filter = TextureFilter::Bilinear;
        m_anisotropic_filter = TextureAnisotropicFilter::None;
        m_mipmap_count = CalculateMipmapCount(width, height);

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

    void OpenGLTextureCubemap::SetWrapMode(TextureWrapMode wrap_mode) {
        m_wrap_mode = wrap_mode;
        u32 wrap = GetGLWrapMode(wrap_mode);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_T, wrap);
        glTextureParameteri(m_texture_id, GL_TEXTURE_WRAP_R, wrap);
    }

    void OpenGLTextureCubemap::SetFilter(TextureFilter filter) {
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

    void OpenGLTextureCubemap::SetAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        f32 amount = 1.0f;
        switch (anisotropic_filter) {
            case TextureAnisotropicFilter::None: amount = 1.0f; break;
            case TextureAnisotropicFilter::Times2: amount = 2.0f; break;
            case TextureAnisotropicFilter::Times4: amount = 4.0f; break;
            case TextureAnisotropicFilter::Times8: amount = 8.0f; break;
            case TextureAnisotropicFilter::Times16: amount = 16.0f; break;
            default: amount = 1.0f; break;
        }
        glTextureParameterf(m_texture_id, GL_TEXTURE_MAX_ANISOTROPY, amount);
    }

    void OpenGLTextureCubemap::SetPixels(TextureCubemapFace face, const u8 *pixels) {
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

        glTextureSubImage3D(m_texture_id, 0, 0, 0, face_offset, m_width, m_height, 1, GetGLFormat(m_format), GL_UNSIGNED_BYTE, pixels);

        // TODO: There should be an option to disable mipmap generation when setting pixels
        glGenerateTextureMipmap(m_texture_id);
    }

    void OpenGLTextureCubemap::CreateTexture(const Map<TextureCubemapFace, const u8 *> &pixels) {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_texture_id);

        bool valid_pixels = pixels.size() == 0 || pixels.size() == 6;
        if (!valid_pixels) {
            HYP_ASSERT_MESSAGE(false, "Can't create cube map from less than 6 faces!");
        }

        SetFilter(m_filter);
        SetWrapMode(m_wrap_mode);
        SetAnisotropicFilter(m_anisotropic_filter);

        glTextureStorage2D(m_texture_id, m_mipmap_count, GetGLInternalFormat(m_format), m_width, m_height);

        if (valid_pixels) {
            for (auto &pair : pixels) {
                SetPixels(pair.first, pair.second);
            }
        }
    }

    u32 OpenGLTextureCubemap::GetGLFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTextureCubemap::GetGLInternalFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTextureCubemap::GetGLWrapMode(TextureWrapMode wrap_mode) {
        switch (wrap_mode) {
            case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::Repeat: return GL_REPEAT;
            case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}