#pragma once

#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/mat3.hpp"
#include "hyperion/core/math/mat4.hpp"

namespace Hyperion::Rendering {

    using MaterialPropertyId = uint64;

    enum class MaterialPropertyType {
        Float32,
        Int32,

        Vec2,
        Vec3,
        Vec4,

        Mat3,
        Mat4,

        Texture
    };

    union MaterialPropertyStorage {
        float32 float32;
        int32 int32;

        Vec2 vec2;
        Vec3 vec3;
        Vec4 vec4;

        Mat3 mat3;
        Mat4 mat4;

        ResourceId texture;
    };

    struct MaterialProperty {
        MaterialPropertyId id;
        MaterialPropertyType type;
        MaterialPropertyStorage storage;

        static MaterialPropertyId NameToId(const String &name) {
            return std::hash<String>{}(name);
        }
    };

}