#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/object/resource.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"
#include "hyperion/rendering/attributes/common_attributes.hpp"
#include "hyperion/rendering/attributes/material_attributes.hpp"
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

        ArrayDescriptor<char> source_vertex;
        ArrayDescriptor<char> source_fragment;
    };

    struct MeshDescriptor {
        ArrayDescriptor<SubMesh> sub_meshes;

        VertexFormat vertex_format;
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
        ResourceId shader_id;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;

        virtual void CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) = 0;
        virtual void DestroyShader(ResourceId shader_id) = 0;

        virtual void CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) = 0;
        virtual void GetTextureData(ResourceId texture_id, Vector<uint8> &data) = 0;
        virtual void DestroyTexture(ResourceId texture_id) = 0;

        virtual void CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) = 0;
        virtual void SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) = 0;
        virtual void DestroyMaterial(ResourceId material_id) = 0;

        virtual void CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) = 0;
        virtual void DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) = 0;
        virtual void DestroyMesh(ResourceId mesh_id) = 0;
    };

}