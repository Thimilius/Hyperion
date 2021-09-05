//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/view/render_view.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(GraphicsContext *graphics_context) {
        graphics_context->CreateDeviceAndSwapChain(&m_device, &m_device_context, &m_swap_chain);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderView *render_view) {
        
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

}