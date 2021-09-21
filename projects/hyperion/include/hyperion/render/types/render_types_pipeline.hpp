#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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

    enum class BlendingOperator {
        Add,
        Subtract,
        ReverseSubract
    };

    struct BlendingFunction {
        BlendingFactor source_factor = BlendingFactor::SourceAlpha;
        BlendingFactor destination_factor = BlendingFactor::InverseSourceAlpha;
        BlendingOperator function_operator = BlendingOperator::Add;
    };

    struct BlendingState {
        bool8 blending_enabled = false;
        BlendingFunction color_function;
        BlendingFunction alpha_function;
    };

    enum class DepthFunction {
        Never,
        Always,

        Less,
        LessEqual,
        Greater,
        GreaterEqual,

        Equal,
        NotEqual
    };

    struct DepthStencilState {
        bool8 depth_enabled = true;
        bool8 depth_write_enabled = true;
        DepthFunction depth_function = DepthFunction::Less;

        bool8 stencil_enabled = false;
    };

    enum class PolygonMode {
        Fill,
        Line
    };

    enum class CullingMode {
        None,
        Front,
        Back,
        FrontAndBack
    };

    enum class CullingFrontFaceMode {
        Clockwise,
        CounterClockwise
    };

    struct RasterizerState {
        PolygonMode polygon_mode = PolygonMode::Fill;

        CullingMode culling_mode = CullingMode::Back;
        CullingFrontFaceMode culling_front_face_mode = CullingFrontFaceMode::Clockwise;
    };

    struct PipelineState {
        BlendingState blending;
        DepthStencilState depth_stencil;
        RasterizerState rasterizer;
    };

}