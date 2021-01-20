#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/resource.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
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

    struct ShaderDescriptor {
        ShaderStageFlags stage_flags;

        String vertex;
        String fragment;
    };

    struct MeshDescriptor {
        Vector<SubMesh> sub_meshes;

        VertexFormat vertex_format;
        IndexFormat index_format;

        Vector<uint8> vertex_data;
        Vector<uint8> index_data;
    };

    struct TextureDescriptor {
        TextureDimension dimension;
        TextureFormat format;
        TextureParameters parameters;
        uint32 mipmap_count;
        TextureSize size;
        Vector<uint8> pixels;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void Viewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;

        virtual void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) = 0;
        virtual void FreeShader(ResourceId id) = 0;

        virtual void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) = 0;
        virtual void FreeMesh(ResourceId id) = 0;

        virtual void CreateTexture(ResourceId id, const TextureDescriptor &descriptor) = 0;
        virtual void FreeTexture(ResourceId id) = 0;

        virtual void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) = 0;
    };

}