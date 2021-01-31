#pragma once

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
        int32 x;
        int32 y;
        int32 width;
        int32 height;
    };

    class CommandBuffer {
    public:
        virtual ~CommandBuffer() = default;

        virtual void ClearCommands() = 0;

        virtual void Clear(ClearFlags clear_flags, Color color) = 0;
        virtual void SetViewport(const Viewport &viewport) = 0;
        virtual void SetRasterizerState(const RasterizerState &rasterizer_state) = 0;

        virtual void DrawMesh(ResourceId mesh_id, ResourceId material_id, uint32 sub_mesh_index) = 0;
    };

}