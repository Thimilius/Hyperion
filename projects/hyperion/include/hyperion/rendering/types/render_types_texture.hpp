#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class TextureDimension {
        Texture2D,
        TextureCubemap
    };

    enum class TextureFormat {
        RGBA32,
        RGB24,
        R8
    };

    enum class TextureWrapMode {
        Clamp,
        Border,
        Repeat,
        MirroredRepeat
    };

    enum class TextureFilter {
        Point,
        Bilinear,
        Trilinear
    };

    enum class TextureAnisotropicFilter {
        None,
        Times2,
        Times4,
        Times8,
        Times16
    };

    enum class RenderTextureFormat {
        RGBA32,
        UInt32,
        Depth24Stencil8,
    };

    struct TextureAttributes {
        TextureWrapMode wrap_mode = TextureWrapMode::Clamp;
        TextureFilter filter = TextureFilter::Bilinear;
        TextureAnisotropicFilter anisotropic_filter = TextureAnisotropicFilter::None;
        bool8 use_mipmaps = true;
    };

}