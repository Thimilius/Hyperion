#include "hyppch.hpp"

#include "hyperion/rendering/texture.hpp"

#include "hyperion/platform/opengl/opengl_texture_2d.hpp"

namespace Hyperion::Rendering {

    TRef<CTexture2D> CTexture2D::Create(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLTexture2D>(width, height, format, wrap_mode, filter, anisotropic_filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CTexture2D> CTexture2D::Create(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter, const u8 *pixels) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLTexture2D>(width, height, format, wrap_mode, filter, anisotropic_filter, pixels);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CTexture2D> CTexture2D::CreateFromFile(const TString &path, ETextureWrapMode wrap_mode, ETextureFilter filter, ETextureAnisotropicFilter anisotropic_filter) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLTexture2D>(path, wrap_mode, filter, anisotropic_filter);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    u32 CTexture::GetBitsPerPixel(ETextureFormat format) {
        switch (format) {
            case Hyperion::Rendering::ETextureFormat::RGB: return 3;
            case Hyperion::Rendering::ETextureFormat::RGBA: return 4;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }
}