#include "hyppch.hpp"

#include "hyperion/rendering/texture.hpp"

#include "hyperion/platform/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    CTexture2D *CTexture2D::Create(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLTexture2D(width, height, mode, wrap_mode, filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}