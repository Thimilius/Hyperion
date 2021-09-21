#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    using MaterialPropertyId = uint64;
    using MaterialPropertyIndex = uint64;

    enum class MaterialPropertyType {
        Unknown,

        Float,
        Int,
        Vector,
        Color,
        Matrix,
    };

    union MaterialPropertyStorage {
        float32 float32;
        int32 int32;
        Vector4 vector4;
        Color color;
        Matrix4x4 matrix4x4;
    };

    struct MaterialProperty {
        MaterialPropertyId id;
        MaterialPropertyType type;
        MaterialPropertyStorage storage = { };

        inline static Rendering::MaterialPropertyId PropertyToId(const String &name) { return std::hash<String>{}(name); }
    };

    using MaterialPropertyCollection = Array<MaterialProperty>;
    using MaterialPropertyIndices = Map<MaterialPropertyId, MaterialPropertyIndex>;

}