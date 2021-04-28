//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/render_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/profiling/profiling.hpp"
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderEngine::PreInitialize(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;

        s_graphics_context = window->CreateGraphicsContext(settings.graphics_backend);
        s_graphics_context->Initialize(GraphicsContextDescriptor());
        s_graphics_context->CreateDeviceAndSwapChain(&s_graphics_device, &s_graphics_device_context, &s_graphics_swap_chain);
    }

    //--------------------------------------------------------------
    void RenderEngine::Initialize() {
        switch (s_render_settings.pipeline) {
            case RenderPipeline::Forward: s_render_pipeline = new ForwardRenderPipeline(); break;
            case RenderPipeline::Custom: s_render_pipeline = s_render_settings.custom_pipeline; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        s_render_pipeline->Initialize();
    }

    //--------------------------------------------------------------
    void RenderEngine::Render() {
        GraphicsRenderPass *render_pass = s_graphics_device->CreateRenderPass(GraphicsRenderPassDescription());
        s_graphics_device->DestroyObject(render_pass);

        s_graphics_context->SwapBuffers();
    }

    //--------------------------------------------------------------
    void RenderEngine::Shutdown() {
        delete s_render_pipeline;

        s_graphics_context->Shutdown();
        delete s_graphics_context;
    }

}