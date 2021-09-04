#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    enum class GraphicsDepthFunction {
        Never,
        Always,

        Less,
        LessEqual,
        Greater,
        GreaterEqual,

        Equal,
        NotEqual
    };

    struct GraphicsDepthStencilStateDescription {
        bool8 depth_enabled = true;
        bool8 depth_write_enabled = true;
        GraphicsDepthFunction depth_function = GraphicsDepthFunction::Less;

        bool8 stencil_enabled = false;
    };

}