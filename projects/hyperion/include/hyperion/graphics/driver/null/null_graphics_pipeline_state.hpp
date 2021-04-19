#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_pipeline_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsPipelineState : public GraphicsPipelineState {
    public:
        NullGraphicsPipelineState(GraphicsDevice *device, const GraphicsPipelineStateDescription &description);
    };

}