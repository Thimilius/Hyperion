#include "hyppch.hpp"

#include "hyperion/rendering/texture_2d.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/driver/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, parameters);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const u8 *pixels) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, parameters, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter) {
        TextureParameters parameters;
        parameters.wrap_mode = wrap_mode;
        parameters.filter = filter;
        parameters.anisotropic_filter = anisotropic_filter;
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, parameters);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Texture2D> Texture2D::Create(u32 width, u32 height, TextureFormat format, TextureWrapMode wrap_mode, TextureFilter filter, TextureAnisotropicFilter anisotropic_filter, const u8 *pixels) {
        TextureParameters parameters;
        parameters.wrap_mode = wrap_mode;
        parameters.filter = filter;
        parameters.anisotropic_filter = anisotropic_filter;
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTexture2D>(width, height, format, parameters, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}