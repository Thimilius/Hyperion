#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/object/resource.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"
#include "hyperion/rendering/attributes/texture_attributes.hpp"

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

    template<typename T>
    struct ArrayDescriptor {
        uint64 size; // This size is always in bytes!
        const T *data;

        ArrayDescriptor() = default;

        ArrayDescriptor(const Vector<T> &vector) {
            size = vector.size() * sizeof(vector[0]);
            data = vector.data();
        }

        ArrayDescriptor(const String &string) {
            size = string.size() + 1; // We have to take into account the null termination character
            data = string.data();
        }
    };

    struct ShaderDescriptor {
        ShaderStageFlags stage_flags;

        ArrayDescriptor<char> source_vertex;
        ArrayDescriptor<char> source_fragment;
    };

    struct VertexAttributeDescriptor {
        VertexAttribute attribute;
        VertexAttributeType type;
        uint32 dimension;
    };

    struct MeshDescriptor {
        ArrayDescriptor<SubMesh> sub_meshes;

        struct VertexFormat {
            ArrayDescriptor<VertexAttributeDescriptor> attributes;
            uint32 stride;
        } vertex_format;
        IndexFormat index_format;

        ArrayDescriptor<uint8> vertices;
        ArrayDescriptor<uint8> indices;
    };

    struct TextureDescriptor {
        TextureDimension dimension;
        TextureFormat format;
        TextureParameters parameters;
        uint32 mipmap_count;
        TextureSize size;

        ArrayDescriptor<uint8> pixels;
    };

    struct MaterialDescriptor {
        ResourceId shader;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void Viewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;

        virtual void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) = 0;
        virtual void DestroyShader(ResourceId id) = 0;

        virtual void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) = 0;
        virtual void DestroyMesh(ResourceId id) = 0;

        virtual void CreateTexture(ResourceId id, const TextureDescriptor &descriptor) = 0;
        virtual void DestroyTexture(ResourceId id) = 0;

        virtual void CreateMaterial(ResourceId id, const MaterialDescriptor &descriptor) = 0;
        virtual void DestroyMaterial(ResourceId id) = 0;

        virtual void DrawIndexed(ResourceId mesh_id, ResourceId material_id) = 0;
    };

}