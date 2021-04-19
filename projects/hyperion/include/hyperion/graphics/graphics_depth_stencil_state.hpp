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
        bool depth_enabled = true;
        bool depth_write_enabled = true;
        GraphicsDepthFunction depth_function = GraphicsDepthFunction::Less;

        bool stencil_enabled = false;
    };

}