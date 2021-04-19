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

    struct GraphicsBlendingFunctionDescription {
        GraphicsBlendingFactor source_factor = GraphicsBlendingFactor::SourceAlpha;
        GraphicsBlendingFactor destination_factor = GraphicsBlendingFactor::InverseSourceAlpha;
    };

    enum class GraphicsBlendingEquation {
        Add,
        Subtract,
        ReverseSubract
    };

    struct GraphicsBlendingStateDescription {
        bool blending_enabled = false;
        GraphicsBlendingFunctionDescription blending_function;
        GraphicsBlendingEquation blending_equation = GraphicsBlendingEquation::Add;
    };

}