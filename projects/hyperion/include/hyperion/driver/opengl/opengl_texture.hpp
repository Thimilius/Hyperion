#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    class OpenGLTexture {
    protected:
        OpenGLTexture() = default;
        virtual ~OpenGLTexture() = default;

        static u32 GetGLFormat(TextureFormat format);
        static u32 GetGLInternalFormat(TextureFormat format);
        static u32 GetGLWrapMode(TextureWrapMode wrap_mode);
        static u32 GetGLMinFilter(TextureFilter filter);
        static u32 GetGLMaxFilter(TextureFilter filter);
        static f32 GetGLAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter);
    };

}