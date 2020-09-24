#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"
#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Rendering {

    enum class ClearMask {
        None,
        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearMask);

    enum class RenderTextureTarget {
        DrawAndRead,
        Draw,
        Read
    };

    class RenderDriver {
    public:
        RasterizerState *GetRasterizerState() { return m_rasterizer_state; }
        
        virtual void Clear(ClearMask mask) = 0;
        virtual void Clear(ClearMask mask, Color color) = 0;

        virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) = 0;

        virtual void SetActiveRenderTarget(RenderTexture *texture, RenderTextureTarget target) = 0;
        virtual void Blit(RenderTexture *destination, u32 dstX0, u32 dstY0, u32 dstX1, u32 dstY1, RenderTexture *source, u32 srcX0, u32 srcY0, u32 srcX1, u32 srcY1) = 0;

        virtual void DrawIndexed(MeshTopology topology, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) = 0;
        virtual void Draw(MeshTopology topology, u32 vertex_count, u32 vertex_offset) = 0;
    protected:
        RasterizerState *m_rasterizer_state;
    };

}