#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"

namespace Hyperion::Rendering {

    enum class ERenderAPI {
        None,
        OpenGL
    };

    enum class EFeature {
        None,
        Culling,
        DepthTesting,
        StencilTesting,
        Blending
    };

    enum class EClearMask {
        None,
        Color   = BIT(0),
        Depth   = BIT(1),
        Stencil = BIT(2)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(EClearMask);

    enum class EBlendFactor {
        None,
        
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

    enum class EBlendEquation {
        None,
        Add,
        Subtract,
        ReverseSubract,
    };

    enum class EFrontFaceMode {
        None,
        Clockwise,
        CounterClockwise
    };

    enum class ECullingMode {
        None,
        Back,
        Front,
        FrontAndBack
    };

    enum class ERenderTextureTarget {
        None,
        DrawAndRead,
        Draw,
        Read
    };

    enum class EPolygonMode {
        None,
        Fill,
        Line
    };

    enum class EPrimitive {
        None,
        Lines,
        LineStrip,
        LineLoop,
        Triangles
    };

    class CRenderAPI {
    private:
        inline static ERenderAPI m_render_api = ERenderAPI::OpenGL;
    public:
        virtual void EnableFeature(EFeature feature) = 0;
        virtual void DisableFeature(EFeature feature) = 0;

        virtual void SetFrontFaceMode(EFrontFaceMode front_face_mode) = 0;
        virtual void SetCullingMode(ECullingMode culling_mode) = 0;

        virtual void SetBlendFunc(EBlendFactor source_factor, EBlendFactor destination_factor) = 0;
        virtual void SetBlendEquation(EBlendEquation blend_equation) = 0;

        virtual void SetPolygonMode(EPolygonMode polygon_mode) = 0;

        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear(EClearMask mask) = 0;

        virtual void SetViewport(s32 x, s32 y, s32 width, s32 height) = 0;

        virtual void SetActiveRenderTarget(const TRef<CRenderTexture> texture, ERenderTextureTarget target) = 0;
        virtual void Blit(const TRef<CRenderTexture> destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const TRef<CRenderTexture> source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) = 0;

        virtual void DrawIndexed(EPrimitive primitive, EIndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) = 0;
        virtual void Draw(EPrimitive primitive, u32 vertex_count, u32 vertex_offset) = 0;
    public:
        inline static ERenderAPI GetAPI() { return m_render_api; }
    };

}