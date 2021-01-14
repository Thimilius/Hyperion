#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/resource.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"

namespace Hyperion::Rendering {

    enum class ClearFlags {
        None,

        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearFlags);

    struct VertexMesh {
        Vec3 position;
        Vec3 normal;
        Vec2 uv;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;

        virtual void CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) = 0;
        virtual void FreeMesh(ResourceId id) = 0;
    };

}