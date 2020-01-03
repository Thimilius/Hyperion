#include "hyppch.hpp"

#include "hyperion/rendering/texture_cubemap.hpp"

#include "hyperion/driver/opengl/opengl_texture_cubemap.hpp"

namespace Hyperion::Rendering {

    Ref<TextureCubemap> TextureCubemap::Create(u32 width, u32 height) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTextureCubemap>(width, height);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<TextureCubemap> TextureCubemap::Create(u32 width, u32 height, const Map<CubemapFace, const u8 *> &pixels) {
        switch (RenderEngine::GetBackend()) {
            case RenderBackend::OpenGL: return std::make_shared<OpenGLTextureCubemap>(width, height, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}