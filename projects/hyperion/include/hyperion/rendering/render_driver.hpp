#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"

namespace Hyperion::Rendering {

    enum class RenderFeature {
        Culling,
        DepthTesting,
        StencilTesting,
        Blending
    };

    enum class ClearMask {
        None,
        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(ClearMask);

    enum class BlendFactor {
        Zero,
        One,

        SourceAlpha,
        SourceColor,
        DestinationAlpha,
        DestinationColor,
        
        InverseSourceAlpha,
        InverseSourceColor,
        InverseDestinationAlpha,
        InverseDestinationColor
    };

    enum class BlendEquation {
        Add,
        Subtract,
        ReverseSubract,
    };

    enum class FrontFaceMode {
        Clockwise,
        CounterClockwise
    };

    enum class CullingMode {
        Back,
        Front,
        FrontAndBack
    };

    enum class RenderTextureTarget {
        DrawAndRead,
        Draw,
        Read
    };

    enum class PolygonMode {
        Fill,
        Line
    };

    enum class Primitive {
        Lines,
        LineStrip,
        LineLoop,
        Triangles
    };

    class RenderDriver {
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void EnableFeature(RenderFeature feature) = 0;
        virtual void DisableFeature(RenderFeature feature) = 0;

        virtual void SetFrontFaceMode(FrontFaceMode front_face_mode) = 0;
        virtual void SetCullingMode(CullingMode culling_mode) = 0;

        virtual void SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) = 0;
        virtual void SetBlendEquation(BlendEquation blend_equation) = 0;

        virtual void SetPolygonMode(PolygonMode polygon_mode) = 0;

        virtual void SetClearColor(f32 r, f32 g, f32 b, f32 a) = 0;
        virtual void Clear(ClearMask mask) = 0;

        virtual void SetViewport(s32 x, s32 y, s32 width, s32 height) = 0;

        virtual void SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target) = 0;
        virtual void Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) = 0;

        virtual void DrawIndexed(Primitive primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) = 0;
        virtual void Draw(Primitive primitive, u32 vertex_count, u32 vertex_offset) = 0;
    };

}