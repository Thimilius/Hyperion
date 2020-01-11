#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/rasterizer_state.hpp"

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

    enum class PrimitiveType {
        Lines,
        LineStrip,
        LineLoop,
        Triangles
    };

    class RenderDriver {
    protected:
        Scope<RasterizerState> m_rasterizer_state;
    public:
        RasterizerState *GetRasterizerState() { return m_rasterizer_state.get(); }

        virtual void Clear(ClearMask mask) = 0;
        virtual void Clear(ClearMask mask, Color color) = 0;

        virtual void SetViewport(s32 x, s32 y, s32 width, s32 height) = 0;

        virtual void SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target) = 0;
        virtual void Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) = 0;

        virtual void DrawIndexed(PrimitiveType primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) = 0;
        virtual void Draw(PrimitiveType primitive, u32 vertex_count, u32 vertex_offset) = 0;
    };

}