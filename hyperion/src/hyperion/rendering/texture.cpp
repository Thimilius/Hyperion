#include "hyppch.hpp"

#include "hyperion/rendering/texture.hpp"

#include "hyperion/platform/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    CTexture2D *CTexture2D::Create(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLTexture2D(width, height, format, wrap_mode, filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    CTexture2D *CTexture2D::Create(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, const u8 *pixels) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLTexture2D(width, height, format, wrap_mode, filter, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    CTexture2D *CTexture2D::CreateFromFile(const TString &path, ETextureWrapMode wrap_mode, ETextureFilter filter) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return new COpenGLTexture2D(path, wrap_mode, filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }
}