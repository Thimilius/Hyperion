#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"
#include "hyperion/graphics/graphics_blending_state.hpp"
#include "hyperion/graphics/graphics_depth_stencil_state.hpp"
#include "hyperion/graphics/graphics_rasterizer_state.hpp"
#include "hyperion/graphics/graphics_render_pass.hpp"
#include "hyperion/graphics/graphics_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsPipelineStateDescription : public GraphicsDeviceObjectDescription {
        GraphicsBlendingStateDescription blending_description;
        GraphicsDepthStencilStateDescription depth_stencil_description;
        GraphicsRasterizerStateDescription rasterizer_description;

        GraphicsRenderPass *render_pass;

        GraphicsShader *vertex_shader;
        GraphicsShader *pixel_shader;
    };

    class GraphicsPipelineState : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsPipelineState() = default;
    protected:
        GraphicsPipelineState(GraphicsDevice *device, const GraphicsPipelineStateDescription &description);
    public:
        virtual const GraphicsPipelineStateDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsPipelineStateDescription m_description;
    };

}