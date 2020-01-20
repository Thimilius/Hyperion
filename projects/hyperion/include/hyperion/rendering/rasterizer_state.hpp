#pragma once

namespace Hyperion::Rendering {
    
    enum class DepthEquation {
        Never,
        Always,

        Less,
        LessEqual,
        Greater,
        GreaterEqual,

        Equal,
        NotEqual
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
        ReverseSubract
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
        bool m_depth_test_enabled;
        bool m_depth_mask_enabled;
        DepthEquation m_depth_equation;

        bool m_stencil_test_enabled;

        bool m_culling_enabled;
        CullingMode m_culling_mode;
        FrontFaceMode m_front_face_mode;

        bool m_blending_enabled;
        BlendFactor m_blending_source_factor;
        BlendFactor m_blending_destination_factor;
        BlendEquation m_blending_equation;

        PolygonMode m_polygon_mode;
    public:
        virtual ~RasterizerState() = default;

        bool IsDepthTestEnabled() const { return m_depth_test_enabled; }
        virtual void SetDepthTestEnabled(bool enabled) = 0;
        bool IsDepthMaskEnabked() const { return m_depth_mask_enabled; }
        virtual void SetDepthMaskEnabled(bool enabled) = 0;
        DepthEquation GetDepthEquation() const { return m_depth_equation; }
        virtual void SetDepthEquation(DepthEquation depth_equation) = 0;

        bool IsStencilTestEnabled() const { return m_stencil_test_enabled; }
        virtual void SetStencilTestEnabled(bool enabled) = 0;

        bool IsCullingEnabled() const { return m_culling_enabled; }
        virtual void SetCullingEnabled(bool enabled) = 0;
        CullingMode GetCullingMode() const { return m_culling_mode; }
        virtual void SetCullingMode(CullingMode culling_mode) = 0;
        FrontFaceMode GetFrontFaceMode() const { return m_front_face_mode; }
        virtual void SetFrontFaceMode(FrontFaceMode front_face_mode) = 0;

        bool IsBlendingEnabled() const { return m_blending_enabled; }
        virtual void SetBlendingEnabled(bool enabled) = 0;
        BlendFactor GetSourceBlendFactor() const { return m_blending_source_factor; }
        BlendFactor GetDestinationBlendFactor() const { return m_blending_destination_factor; }
        virtual void SetBlendingFunc(BlendFactor source_factor, BlendFactor destination_factor) = 0;
        BlendEquation GetBlendEquation() const { return m_blending_equation; }
        virtual void SetBlendingEquation(BlendEquation blend_equation) = 0;

        PolygonMode GetPolygonMode() const { return m_polygon_mode; }
        virtual void SetPolygonMode(PolygonMode polygon_mode) = 0;
    };

}