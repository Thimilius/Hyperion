#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {
    
    enum class GraphicsPolygonMode {
        Fill,
        Line
    };

    enum class GraphicsCullingMode {
        None,
        Front,
        Back,
        FrontAndBack
    };

    enum class GraphicsCullingFrontFaceMode {
        Clockwise,
        CounterClockwise
    };

    struct GraphicsRasterizerStateDescription {
        GraphicsPolygonMode polygon_mode = GraphicsPolygonMode::Fill;

        GraphicsCullingMode culling_mode = GraphicsCullingMode::Back;
        GraphicsCullingFrontFaceMode culling_front_face_mode = GraphicsCullingFrontFaceMode::Clockwise;
    };

}