#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    enum class GraphicsShaderStageFlags {
        Unknown,

        Vertex = BIT(0),
        Fragment = BIT(1),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(GraphicsShaderStageFlags);

    enum class GraphicsTextureDimension {
        Texture2D,
        TextureCubemap
    };

    enum class GraphicsTextureFormat {
        RGBA32,
        RGB24,
        R8
    };

    enum class GraphicsTextureWrapMode {
        Clamp,
        Border,
        Repeat,
        MirroredRepeat
    };

    enum class GraphicsTextureFilter {
        Point,
        Bilinear,
        Trilinear
    };

    enum class GraphicsTextureAnisotropicFilter {
        None,
        Times2,
        Times4,
        Times8,
        Times16
    };

    enum class GraphicsRenderTextureFormat {
        RGBA32,
        UInt32,
        Depth24Stencil8,
    };

    enum class GraphicsIndexFormat {
        UInt16,
        UInt32
    };

    enum class GraphicsTopology {
        Points,
        Lines,
        LineStrip,
        Triangles
    };

}