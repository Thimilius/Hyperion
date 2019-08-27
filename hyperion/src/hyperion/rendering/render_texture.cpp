#include "hyppch.hpp"

#include "hyperion/rendering/render_texture.hpp"

#include "hyperion/platform/opengl/opengl_render_texture.hpp"

namespace Hyperion::Rendering {

    CRenderTexture *CRenderTexture::Create(u32 width, u32 height, ERenderTextureFormat format) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLRenderTexture(width, height, format);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}