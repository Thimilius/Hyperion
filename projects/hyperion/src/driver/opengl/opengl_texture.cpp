#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_texture.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    u32 OpenGLTexture::GetGLFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLTexture::GetGLInternalFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
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