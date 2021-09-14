#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class CameraClearMode {
        Nothing,
        Depth,
        Color,
        Skybox
    };

    enum class CameraProjectionMode {
        Perspective,
        Orthographic
    };

    struct CameraViewport {
        uint32 x;
        uint32 y;
        uint32 width;
        uint32 height;
    };

    struct CameraViewportClipping {
        float32 x;
        float32 y;
        float32 width;
        float32 height;
    };

    enum class VertexAttributeKind {
        Position,
        Normal,
        Tangent,
        Color,
        Texture0,
        Texture1,
        Texture2,
        Texture3,
    };

    enum class VertexAttributeType {
        Float32,
    };

    struct VertexAttribute {
        VertexAttributeKind kind;
        VertexAttributeType type;
        uint32 dimension;
    };

    enum class IndexFormat {
        UInt16,
        UInt32
    };

    enum class MeshTopology {
        Points,
        Lines,
        LineStrip,
        Triangles
    };

    struct SubMesh {
        MeshTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

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

    struct TextureAttributes {
        TextureWrapMode wrap_mode = TextureWrapMode::Clamp;
        TextureFilter filter = TextureFilter::Bilinear;
        TextureAnisotropicFilter anisotropic_filter = TextureAnisotropicFilter::None;
        bool8 use_mipmaps = true;
    };

    struct TextureSize {
        uint32 width;
        uint32 height;
    };

    enum class RenderTextureFormat {
        RGBA32,
        UInt32,
        Depth24Stencil8,
    };

    struct RenderTextureAttachment {
        RenderTextureFormat format;
        TextureAttributes parameters;
    };

}