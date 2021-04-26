//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_render_pass.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsRenderPass::GraphicsRenderPass(GraphicsDevice *device, const GraphicsRenderPassDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}