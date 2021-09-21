#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    using ShaderPropertyId = uint64;

    enum class ShaderStageFlags {
        Unknown,

        Vertex = BIT(0),
        Fragment = BIT(1),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

    enum class ShaderPropertyType {
        Unknown,

        Float,
        Int,
        Vector,
        Color,
        Matrix,
    };

    enum class ShaderLightMode {
        Unknown,

        None,
        Forward,
    };

    union ShaderPropertyStorage {
        float32 float32;
        int32 int32;
        Vector4 vector4;
        Color color;
        Matrix4x4 matrix4x4;
    };

    struct ShaderProperty {
        String name;

        ShaderPropertyId id;
        ShaderPropertyType type;
        ShaderPropertyStorage storage = { };

        inline static ShaderPropertyId PropertyToId(const String &name) { return std::hash<String>{}(name); }
    };

    struct ShaderAttributes {
        ShaderLightMode light_mode = ShaderLightMode::None;
    };

    using ShaderPropertyCollection = Array<ShaderProperty>;

    struct ShaderData {
        ShaderAttributes attributes;
        ShaderPropertyCollection properties;

        ShaderStageFlags stage_flags;

        String vertex_source;
        String fragment_source;
    };

}