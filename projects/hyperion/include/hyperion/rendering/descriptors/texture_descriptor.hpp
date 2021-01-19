#pragma once

namespace Hyperion::Rendering {

    enum class TextureDimension {
        Texture2D,
        TextureCubemap
    };

    enum class TextureFormat {
        RGB24,
        RGBA32,
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
        TextureWrapMode wrap_mode = TextureWrapMode::Clamp;
        TextureFilter filter = TextureFilter::Bilinear;
        TextureAnisotropicFilter anisotropic_filter = TextureAnisotropicFilter::None;
        bool use_mipmaps = true;
    };

    struct TextureSize {
        uint32 width; // Width for 2D and cubemap textures
        uint32 height; // Height for 2D and cubemap textures
        // NOTE: For other supported texture types we can add more required values here (3D textures and texture arrays)
    };

    struct TextureDescriptor {
        TextureDimension dimension = TextureDimension::Texture2D;
        TextureFormat format = TextureFormat::RGBA32;
        TextureParameters parameters;
        uint32 mipmap_count = 1;
        TextureSize size;
        Vector<uint8> pixels;
    };

}