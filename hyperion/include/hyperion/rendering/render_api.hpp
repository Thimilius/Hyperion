#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/enum.hpp"
#include "vertex_array.hpp"

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

        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear(EClearMask mask) = 0;

        virtual void SetViewport(s32 x, s32 y, s32 width, s32 height) = 0;
        
        virtual void DrawIndexed(const TRef<CVertexArray> &vertex_array) = 0;
    public:
        inline static ERenderAPI GetAPI() { return m_render_api; }
    };

}