#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_object.hpp"
#include "hyperion/graphics/graphics_blending_state.hpp"
#include "hyperion/graphics/graphics_depth_stencil_state.hpp"
#include "hyperion/graphics/graphics_rasterizer_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsPipelineStateDescription : public GraphicsObjectDescription {
        GraphicsBlendingStateDescription blending_description;
        GraphicsDepthStencilStateDescription depth_stencil_description;
        GraphicsRasterizerStateDescription rasterizer_description;
    };

    class IGraphicsPipelineState : public IGraphicsObject {
    public:
        virtual ~IGraphicsPipelineState() = default;
    public:
        virtual const GraphicsPipelineStateDescription &GetDescription() const override = 0;
    };

}