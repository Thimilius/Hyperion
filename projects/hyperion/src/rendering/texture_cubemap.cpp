#include "hyppch.hpp"

#include "hyperion/rendering/texture_cubemap.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/driver/opengl/opengl_texture_cubemap.hpp"

namespace Hyperion::Rendering {

    Ref<TextureCubemap> TextureCubemap::Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTextureCubemap>(width, height, format, parameters);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<TextureCubemap> TextureCubemap::Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const Map<TextureCubemapFace, const u8 *> &pixels) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTextureCubemap>(width, height, format, parameters, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}