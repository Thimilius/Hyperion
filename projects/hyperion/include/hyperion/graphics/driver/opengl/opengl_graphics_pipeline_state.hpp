#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_pipeline_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsPipelineState : public GraphicsPipelineState {
    public:
        OpenGLGraphicsPipelineState(GraphicsDevice *device, const GraphicsPipelineStateDescription &description);
    };

}