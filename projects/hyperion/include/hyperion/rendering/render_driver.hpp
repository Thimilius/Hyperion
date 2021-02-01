#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/object/resource.hpp"
#include "hyperion/rendering/graphics_context.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"
#include "hyperion/rendering/attributes/common_attributes.hpp"
#include "hyperion/rendering/attributes/material_attributes.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"
#include "hyperion/rendering/attributes/texture_attributes.hpp"

namespace Hyperion::Rendering {

    enum class ClearFlags {
        None,

        Color = BIT(0),
        Depth = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearFlags);

    struct Viewport {
        uint32 x;
        uint32 y;
        uint32 width;
        uint32 height;
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

    // TODO: Add support for msaa.
    struct RenderTextureDescriptor {
        TextureSize size;
        uint32 mipmap_count;

        ArrayDescriptor<RenderTextureAttachment> attachments;
    };

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;

        virtual void Initialize(GraphicsContext *graphics_context) = 0;
        virtual void Shutdown() = 0;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;
        virtual void SetCameraData(const CameraData &camera_data) = 0;

        virtual void CreateShader(ResourceId shader_id, const ShaderDescriptor &descriptor) = 0;
        virtual void DestroyShader(ResourceId shader_id) = 0;

        virtual void CreateTexture(ResourceId texture_id, const TextureDescriptor &descriptor) = 0;
        virtual void GetTextureData(ResourceId texture_id, Vector<uint8> &data) = 0;
        virtual void DestroyTexture(ResourceId texture_id) = 0;

        virtual void CreateMaterial(ResourceId material_id, const MaterialDescriptor &descriptor) = 0;
        virtual void SetMaterialProperty(ResourceId material_id, const MaterialProperty &property) = 0;
        virtual void DestroyMaterial(ResourceId material_id) = 0;

        virtual void CreateRenderTexture(ResourceId render_texture_id, const RenderTextureDescriptor &descriptor) = 0;
        virtual void ResizeRenderTexture(ResourceId render_texture_id, uint32 width, uint32 height, uint32 mipmap_count) = 0;
        virtual void SetRenderTexture(ResourceId render_texture_id) = 0;
        virtual void BlitRenderTexture(ResourceId destination_id, uint32 destination_width, uint32 destination_height, ResourceId source_id, uint32 source_width, uint32 source_height) = 0;
        virtual void DestroyRenderTexture(ResourceId render_texture_id) = 0;

        virtual void CreateMesh(ResourceId mesh_id, const MeshDescriptor &descriptor) = 0;
        virtual void DrawMesh(ResourceId mesh_id, const Mat4 &model_matrix, ResourceId material_id, uint32 sub_mesh_index) = 0;
        virtual void DestroyMesh(ResourceId mesh_id) = 0;
    };

}