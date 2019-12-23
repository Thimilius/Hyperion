#include "hyppch.hpp"

#include "hyperion/rendering/texture.hpp"

#include "hyperion/driver/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, wrap_mode, filter, anisotropic_filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, wrap_mode, filter, anisotropic_filter, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::CreateFromFile(const String &path, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(path, wrap_mode, filter, anisotropic_filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    u32 Texture::GetBytesPerPixel(TextureFormat format) {
        switch (format) {
            case Hyperion::Rendering::TextureFormat::RGB: return 3;
            case Hyperion::Rendering::TextureFormat::RGBA: return 4;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }
}