#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/matrix3x3.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/graphics/graphics_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct TextureAttributes {
        Graphics::GraphicsTextureWrapMode wrap_mode = Graphics::GraphicsTextureWrapMode::Clamp;
        Graphics::GraphicsTextureFilter filter = Graphics::GraphicsTextureFilter::Bilinear;
        Graphics::GraphicsTextureAnisotropicFilter anisotropic_filter = Graphics::GraphicsTextureAnisotropicFilter::None;
        bool8 use_mipmaps = true;
    };

    enum class ShaderLightMode {
        Unknown,

        None,
        Forward,
    };

    struct ShaderAttributes {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

    struct ShaderData {
        ShaderAttributes attributes;
        Graphics::GraphicsShaderStageFlags stage_flags;

        String vertex_source;
        String pixel_source;
    };

    using MaterialPropertyId = uint64;

    enum class MaterialPropertyType {
        Float32,

        Int32,
        UInt32,

        Vector2,
        Vector3,
        Vector4,

        Matrix3x3,
        Matrix4x4,

        Color,
    };

    union MaterialPropertyStorage {
        float32 float32;

        int32 int32;
        uint32 uint32;

        Vector2 vec2;
        Vector3 vec3;
        Vector4 vec4;

        Matrix3x3 mat3;
        Matrix4x4 mat4;

        Color color;
    };

    struct MaterialProperty {
        MaterialPropertyId id;
        MaterialPropertyType type;
        MaterialPropertyStorage storage = { };

        inline static MaterialPropertyId NameToId(const String &name) { return std::hash<String>{}(name); }
    };

    using MaterialPropertyCollection = Array<MaterialProperty>;

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

    struct SubMesh {
        Graphics::GraphicsTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

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

}