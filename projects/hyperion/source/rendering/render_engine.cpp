//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/render_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window.hpp"
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"
#include "hyperion/rendering/threading/render_thread_synchronization.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderEngine::PreInitialize(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;

        switch (s_render_settings.pipeline) {
            case RenderPipeline::Forward: s_render_pipeline = new ForwardRenderPipeline(); break;
            case RenderPipeline::Custom: s_render_pipeline = s_render_settings.custom_pipeline; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        s_main_frame = &s_first_frame;
        s_render_frame = &s_second_frame;

        switch (settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                InitializeGraphicsContext(window);
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                s_render_thread.Start(RT_Loop, window);
                s_render_thread.SetName("Render Thread");

                RenderThreadSynchronization::NotifyMainReady();
                RenderThreadSynchronization::WaitForRenderReady();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    //--------------------------------------------------------------
    void RenderEngine::Initialize() {
        s_render_pipeline->Initialize(s_graphics_context);
    }

    //--------------------------------------------------------------
    void RenderEngine::Render() {
        switch (s_render_settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                SynchronizeMainAndRenderThread();
                s_render_pipeline->Render(s_render_frame);
                s_graphics_context->SwapBuffers();
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                RenderThreadSynchronization::WaitForRenderDone();
                SynchronizeMainAndRenderThread();
                RenderThreadSynchronization::NotifySwapDone();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    //--------------------------------------------------------------
    void RenderEngine::Shutdown() {
        s_render_pipeline->Shutdown();
        delete s_render_pipeline;

        switch (s_render_settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                ShutdownGraphicsContext();
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                s_render_thread_should_exit = true;
                RenderThreadSynchronization::NotifySwapDone();
                s_render_thread.Join();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    //--------------------------------------------------------------
    void RenderEngine::RequestExit() {
        s_render_thread_should_exit = true;
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Initialize(Window *window) {
        InitializeGraphicsContext(window);

        RenderThreadSynchronization::NotifyRenderReady();
        RenderThreadSynchronization::WaitForMainReady();
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Loop(void *parameter) {
        RT_Initialize(static_cast<Window *>(parameter));
        
        while (true) {
            if (s_render_thread_should_exit) {
                break;
            }

            s_render_pipeline->Render(s_render_frame);

            {
                s_graphics_context->SwapBuffers();
                RenderThreadSynchronization::NotifyRenderDone();
            }
            
            RenderThreadSynchronization::WaitForSwapDone();
        }

        RT_Shutdown();
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Shutdown() {
        ShutdownGraphicsContext();
    }

    //--------------------------------------------------------------
    void RenderEngine::SynchronizeMainAndRenderThread() {
        RenderFrame *temp = s_main_frame;
        s_main_frame = s_render_frame;
        s_render_frame = temp;

        s_main_frame->Reset();
    }

    //--------------------------------------------------------------
    void RenderEngine::InitializeGraphicsContext(Window *window) {
        s_graphics_context = window->CreateGraphicsContext(s_render_settings.graphics_backend);
        s_graphics_context->Initialize(GraphicsContextDescriptor());
        s_graphics_context->SetVSyncMode(VSyncMode::DontSync);
    }

    void RenderEngine::ShutdownGraphicsContext() {
        s_graphics_context->Shutdown();
        delete s_graphics_context;
    }

}