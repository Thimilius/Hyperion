#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    void OpenGLTexture::SetUnpackAlignmentForFormat(TextureFormat format) {
        u32 alignment = 4;
        switch (format) {
            case Hyperion::Rendering::TextureFormat::RGB24: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::RGBA32: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::R8: alignment = 1; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }

    u32 OpenGLTexture::GetGLFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB24: return GL_RGB;
            case TextureFormat::RGBA32: return GL_RGBA;
            case TextureFormat::R8: return GL_RED;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture::GetGLFormatType(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB24: return GL_UNSIGNED_BYTE;
            case TextureFormat::RGBA32: return GL_UNSIGNED_BYTE;
            case TextureFormat::R8: return GL_UNSIGNED_BYTE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture::GetGLInternalFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB24: return GL_RGB8;
            case TextureFormat::RGBA32: return GL_RGBA8;
            case TextureFormat::R8: return GL_R8;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture::GetGLWrapMode(TextureWrapMode wrap_mode) {
        switch (wrap_mode) {
            case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::Repeat: return GL_REPEAT;
            case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture::GetGLMinFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST_MIPMAP_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR_MIPMAP_NEAREST;
            case TextureFilter::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST_MIPMAP_NEAREST;
        }
    }

    u32 OpenGLTexture::GetGLMaxFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR;
            case TextureFilter::Trilinear: return GL_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST;
        }
    }

    f32 OpenGLTexture::GetGLAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        switch (anisotropic_filter) {
            case TextureAnisotropicFilter::None: return 1.0f;
            case TextureAnisotropicFilter::Times2: return 2.0f;
            case TextureAnisotropicFilter::Times4: return 4.0f;
            case TextureAnisotropicFilter::Times8: return 8.0f;
            case TextureAnisotropicFilter::Times16: return 16.0f;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 1.0f;
        }
    }

}