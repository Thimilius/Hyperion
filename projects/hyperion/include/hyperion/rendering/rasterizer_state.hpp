#pragma once

namespace Hyperion {

    enum class RenderFeature {
        Culling,
        DepthTesting,
        StencilTesting,
        Blending
    };

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

    enum class PolygonMode {
        Fill,
        Line
    };

    class RasterizerState {
    protected:
        FrontFaceMode m_front_face_mode;
        CullingMode m_culling_mode;

        BlendFactor m_source_blend_factor;
        BlendFactor m_destination_blend_factor;
        BlendEquation m_blend_equation;

        PolygonMode m_polygon_mode;
    public:
        FrontFaceMode GetFrontFaceMode() const { return m_front_face_mode; }
        CullingMode GetCullingMode() const { return m_culling_mode; }
        BlendFactor GetSourceBlendFactor() const { return m_source_blend_factor; }
        BlendFactor GetDestinationBlendFactor() const { return m_destination_blend_factor; }
        BlendEquation GetBlendEquation() const { return m_blend_equation; }
        PolygonMode GetPolygonMode() const { return m_polygon_mode; }

        virtual void EnableFeature(RenderFeature feature) = 0;
        virtual void DisableFeature(RenderFeature feature) = 0;
        
        virtual void SetFrontFaceMode(FrontFaceMode front_face_mode) = 0;
        virtual void SetCullingMode(CullingMode culling_mode) = 0;

        virtual void SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) = 0;
        virtual void SetBlendEquation(BlendEquation blend_equation) = 0;

        virtual void SetPolygonMode(PolygonMode polygon_mode) = 0;
    };

}