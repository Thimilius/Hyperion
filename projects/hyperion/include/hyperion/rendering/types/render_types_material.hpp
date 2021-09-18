#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/matrix3x3.hpp"
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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

}