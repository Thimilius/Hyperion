//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_render_pass.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsRenderPass::NullGraphicsRenderPass(GraphicsDevice *device, const GraphicsRenderPassDescription &description) : GraphicsRenderPass(device, description) {

    }

}