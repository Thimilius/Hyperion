//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_pipeline_state.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsPipelineState::GraphicsPipelineState(GraphicsDevice *device, const GraphicsPipelineStateDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}