#pragma once

#include "hyperion/assets/mesh_descriptor.hpp"
#include "hyperion/assets/shader_descriptor.hpp"
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

    struct Viewport {
        int32 x;
        int32 y;
        int32 width;
        int32 height;
    };

    struct MeshDescriptor {
        Vector<VertexAttributeDescriptor> vertex_attributes;

        IndexFormat index_format;
        uint32 index_count;

        Vector<uint8> vertex_data;
        Vector<uint8> index_data;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void Viewport(const Viewport &viewport) = 0;

        virtual void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) = 0;
        virtual void FreeShader(ResourceId id) = 0;

        virtual void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) = 0;
        virtual void FreeMesh(ResourceId id) = 0;

        virtual void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) = 0;
    };

}