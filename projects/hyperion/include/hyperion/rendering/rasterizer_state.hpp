#pragma once

//-------------------- Definition Namespace --------------------
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

    enum class BlendingFactor {
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

    struct BlendingFunction {
        BlendingFactor source_factor = BlendingFactor::SourceAlpha;
        BlendingFactor destination_factor = BlendingFactor::InverseSourceAlpha;
    };

    enum class BlendingEquation {
        Add,
        Subtract,
        ReverseSubract
    };

    enum class CullingMode {
        Back,
        Front,
        FrontAndBack
    };

    enum class CullingFrontFaceMode {
        Clockwise,
        CounterClockwise
    };

    enum class PolygonMode {
        Fill,
        Line
    };

    struct RasterizerState {
        bool depth_test_enabled = true;
        bool depth_mask_enabled = true;
        DepthEquation depth_equation = DepthEquation::Less;

        bool stencil_test_enabled = false;

        bool blending_enabled = false;
        BlendingFunction blending_function;
        BlendingEquation blending_equation = BlendingEquation::Add;

        bool culling_enabled = true;
        CullingMode culling_mode = CullingMode::Back;
        CullingFrontFaceMode culling_front_face_mode = CullingFrontFaceMode::Clockwise;

        PolygonMode polygon_mode = PolygonMode::Fill;
    };

}