#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {
    
    enum class GraphicsCullingMode {
        Back,
        Front,
        FrontAndBack
    };

    enum class GraphicsCullingFrontFaceMode {
        Clockwise,
        CounterClockwise
    };

    enum class GraphicsPolygonMode {
        Fill,
        Line
    };

    struct GraphicsRasterizerStateDescription {
        bool culling_enabled = true;
        GraphicsCullingMode culling_mode = GraphicsCullingMode::Back;
        GraphicsCullingFrontFaceMode culling_front_face_mode = GraphicsCullingFrontFaceMode::Clockwise;

        GraphicsPolygonMode polygon_mode = GraphicsPolygonMode::Fill;
    };

}