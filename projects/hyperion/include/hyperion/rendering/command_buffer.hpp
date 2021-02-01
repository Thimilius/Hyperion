#pragma once

#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/rendering/camera_data.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"

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

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;

        virtual void ClearCommands() = 0;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;

        virtual void SetupCameraData(const CameraData &camera_data) = 0;

        virtual void SetRenderTexture(RenderTexture *render_texture) = 0;
        virtual void Blit(RenderTexture *destination, RenderTexture *source) = 0;
        
        virtual void DrawMesh(Mesh *mesh, const Mat4 &model_matrix, Material *material, uint32 sub_mesh_index) = 0;
    };

}