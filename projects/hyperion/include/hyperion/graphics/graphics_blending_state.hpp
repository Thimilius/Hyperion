#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    enum class GraphicsBlendingFactor {
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

    enum class GraphicsBlendingOperator {
        Add,
        Subtract,
        ReverseSubract
    };

    struct GraphicsBlendingFunctionDescription {
        GraphicsBlendingFactor source_factor = GraphicsBlendingFactor::SourceAlpha;
        GraphicsBlendingFactor destination_factor = GraphicsBlendingFactor::InverseSourceAlpha;
        GraphicsBlendingOperator function_operator = GraphicsBlendingOperator::Add;
    };

    struct GraphicsBlendingStateDescription {
        bool blending_enabled = false;
        GraphicsBlendingFunctionDescription color_function;
        GraphicsBlendingFunctionDescription alpha_function;
    };

}