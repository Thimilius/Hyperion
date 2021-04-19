//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_pipeline_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsPipelineState::NullGraphicsPipelineState(GraphicsDevice *device, const GraphicsPipelineStateDescription &description) : GraphicsPipelineState(device, description) {

    }

}