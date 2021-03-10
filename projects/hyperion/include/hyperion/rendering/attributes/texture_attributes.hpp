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

    struct TextureParameters {
        // TODO: Implement specific wrap modes for the different axes.
        TextureWrapMode wrap_mode = TextureWrapMode::Clamp;
        TextureFilter filter = TextureFilter::Bilinear;
        TextureAnisotropicFilter anisotropic_filter = TextureAnisotropicFilter::None;
        bool use_mipmaps = true;

        inline static TextureParameters ForDefaultTextures() { return { TextureWrapMode::Clamp, TextureFilter::Bilinear, TextureAnisotropicFilter::None, true }; }
        inline static TextureParameters ForRenderTextures() { return { TextureWrapMode::Clamp, TextureFilter::Point, TextureAnisotropicFilter::None, false }; }
    };

    struct TextureSize {
        uint32 width; // Width for 2D and cubemap textures.
        uint32 height; // Height for 2D and cubemap textures.
        // NOTE: For other supported texture types we can add more required values here (3D textures and texture arrays).
    };

    enum class RenderTextureFormat {
        RGBA32,

        UInt32,

        Depth24Stencil8,
    };

    struct RenderTextureAttachment {
        RenderTextureFormat format;
        TextureParameters parameters;
    };

}