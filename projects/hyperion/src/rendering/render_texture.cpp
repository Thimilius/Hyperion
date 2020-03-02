#include "hyppch.hpp"

#include "hyperion/rendering/render_texture.hpp"

#include "hyperion/driver/opengl/opengl_render_texture.hpp"

namespace Hyperion::Rendering {

    Ref<RenderTexture> RenderTexture::Create(u32 width, u32 height, RenderTextureFormat format) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLRenderTexture>(width, height, format);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TextureFormat RenderTexture::GetTextureFormat(RenderTextureFormat format) {
        switch (format) {
            case Hyperion::Rendering::RenderTextureFormat::RGBA8: return TextureFormat::RGBA32;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return TextureFormat::RGBA32;
        };
    }

}