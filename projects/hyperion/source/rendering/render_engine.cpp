//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/render_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"
#include "hyperion/rendering/threading/render_thread_synchronization.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    // ────────────────────────────────────────────────────────────────────────────────────
    // INTRODUCTION:
    // This is the documentation of how the rendering (should/will) work.
    // First of all Hyperion is not an attempt to be the next Unreal Engine, Cry Engine or Unity (you name it).
    // That means that performance is probably not the highest it could be but we still try to make it fairly efficient.
    // Mostly however we want to provide a good high level rendering abstraction layer that is customizable.
    // It should allow one to have different kinds of rendering pipelines (e.g. forward/deferred) if required.
    // 
    // Now onto the technical side of things...
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // PIPELINED MULTITHREADING:
    // Like most engines Hyperion uses a "Main" and a "Render" Thread that are synchronized in a pipelined fashion.
    // The Main Thread does input, simulation, etc. of the current frame while the Render Thread renders the last frame.
    // That means the Render Thread is always rendering one frame behind the Main Thread.
    // The Main Thread extracts the necessary rendering data from the ECS and generates high level rendering commands.
    // When a frame ends on the Main Thread there is a synchronization point where the data is exchanged.
    // The Render Thread takes the exchanged rendering commands and translates them to commands for the underlying graphics API.
    // 
    // An ideal timeline of the pipelined frame processing should look like this:
    //                ┌─────────────┐┌─────────────┐┌─────────────┐
    // Main Thread:   │   Frame N   ││ Frame N + 1 ││ Frame N + 2 │
    //                └─────────────┘└─────────────┘└─────────────┘
    //                ┌─────────────┐┌─────────────┐┌─────────────┐
    // Render Thread: │ Frame N - 1 ││   Frame N   ││ Frame N + 1 │
    //                └─────────────┘└─────────────┘└─────────────┘
    // Obviously in a lot of scenarios the work between the two threads is not evenly divided.
    // That means most of the time either the Main Thread has to wait for the Render Thread (the usual case) or the other way around.
    // 
    // There is also an option to turn off the pipelined multithreading and have everything done on the Main Thread.
    // The data extraction and command translation stays the same regardless, it's just on a single thread.
    // 
    // NOTE: Currently we do not employ any kind of worker threads on the Main or Render Thread.
    // That would of course help with performance for some workloads... but the first paragraph still stands.
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // RENDER FRAME CONTEXT:
    // The RenderFrameContext is the interaction point between the ECS and the rendering engine.
    // Render systems take the different components and extract every object, camera, light, etc. and copies it into the context.
    // It can be thought of as the snapshot of the simulated world with all the data required to render it.
    // 
    // The RenderFrameContext also contains every asset that needs to be loaded/unloaded on the GPU.
    // That includes meshes, shaders, materials, textures, ...
    // The data for shaders and materials is always copied which is usually fine as it is quite lightweight.
    // The bigger data from meshes and textures is ONLY copied when the asset is set to AssetDataAccess::ReadAndWrite.
    // In most cases the asset is set to AssetDataAccess::None which means the data can simply be moved and no copy has to be made.
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // RENDER FRAME:
    // The RenderFrame is the high level structure that gets exchanged between the Main and Render Thread.
    // There are always two static RenderFrames allocated.
    // One used by the Main Thread to fill it with data and the other one used by the Render Thread to read from it.
    // It acts as a double buffer so that the synchronization point between the two threads is therefore a simple pointer swap.
    // 
    // The RenderFrame owns the RenderFrameContext as that contains all relevant ECS data and asset references.
    // That alone however is not enough as we do not yet know HOW to render the data in the context.
    // Because of that another purpose for the RenderFrame is to act as a command buffer for high level render commands.
    // This is mainly inspired by the Scriptable Render Pipeline from Unity.
    // The commands include among other things:
    //     - Drawing:
    //         - General (All objects)
    //         - Detailed (Single object)
    //     - Setting up camera properties
    //     - Rasterizer state
    //     - Render passes
    //     - Render targets:
    //         - Setting
    //         - Blitting
    // NOTE: Currently there is no frustum culling at all.
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // RENDER PIPELINE:
    // The RenderPipeline is the highest level in the rendering API stack.
    // A RenderPipeline lives on the Main Thread and gets passed the corresponding RenderFrame for writing.
    // It uses the RenderFrame to schedule the high level rendering commands which will produce the final render output.
    // That way different kinds of rendering pipelines, like forward or deferred, can easily be implemented.
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // RENDER DRIVER:
    // The RenderDriver is the lowest point in the rendering API stack.
    // A RenderDriver lives on the Render Thread and gets passed the corresponding RenderFrame for reading.
    // For every underlying graphics API there exists a RenderDriver whose job is twofold:
    //     1. Loading/Unloading of GPU assets taken from RenderFrameContext
    //     2. Translation of RenderFrame commands to graphics API commands
    // 
    // NOTE: There was a plan to have a full grown graphics API abstraction layer with implementations for every graphics API.
    // However that would introduce yet ANOTHER API level and might also be very time consuming to develop if we want something efficient. 
    // By having just the RenderDriver we can act close to the engine and its required rendering features.
    // That keeps the actual implementation of the graphics API and all its specifics local and enclosed.
    // This is mainly inspired by Godot which also employs that exact concept.
    // 
    // ────────────────────────────────────────────────────────────────────────────────────
    // RENDER DRIVER CONTEXT:
    // The RenderDriverContext represents the graphics context of the underlying API.
    // It's mainly responsible for initializing the graphics API, loading extensions and presenting the final render output.
    // The RenderDriverContext gets implemented by every graphics API and additionally on a lower level by the underlying platform.
    // ────────────────────────────────────────────────────────────────────────────────────

    //--------------------------------------------------------------
    void RenderEngine::SetVSyncMode(VSyncMode vsync_mode) {
        s_vsync_mode = vsync_mode;

        if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
            s_render_driver_context->SetVSyncMode(s_vsync_mode);
        }
    }

    //--------------------------------------------------------------
    void RenderEngine::PreInitialize(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;
        s_vsync_mode = settings.vsync_mode;

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
        RenderGizmos::Initialize();

        s_render_pipeline->Initialize();
    }

    //--------------------------------------------------------------
    void RenderEngine::Render() {
        {
            HYP_PROFILE_SCOPE("RenderEngine.RenderPipeline");
            s_render_pipeline->Render(s_main_frame);
        }

        switch (s_render_settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                SwapRenderFrames();
                RenderDriver();

                {
                    HYP_PROFILE_SCOPE("RenderEngine.Present");
                    s_render_driver_context->SwapBuffers();
                }
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                {
                    HYP_PROFILE_CATEGORY("RenderEngine.WaitForRenderDone", ProfileCategory::Wait);
                    RenderThreadSynchronization::WaitForRenderDone();
                }
                SwapRenderFrames();
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
    void RenderEngine::RenderDriver() {
        HYP_PROFILE_SCOPE("RenderEngine.RenderDriver");

        IRenderDriver *render_driver = s_render_driver_context->GetDriver();
        render_driver->ResetStats();
        render_driver->Render(s_render_frame);
    }

    //--------------------------------------------------------------
    void RenderEngine::SwapRenderFrames() {
        HYP_PROFILE_SCOPE("RenderEngine.SwapRenderFrames");

        RenderFrame *temp = s_main_frame;
        s_main_frame = s_render_frame;
        s_render_frame = temp;

        s_main_frame->Clear();

        s_render_stats = s_render_driver_context->GetDriver()->GetStats();
    }

    //--------------------------------------------------------------
    void RenderEngine::InitializeGraphicsContext(Window *window) {
        s_render_driver_context = window->CreateRenderDriverContext(s_render_settings.backend);
        s_render_driver_context->Initialize(RenderDriverContextDescriptor());
        s_render_driver_context->SetVSyncMode(s_vsync_mode);
        s_render_driver_context->GetDriver()->Initialize();
    }

    //--------------------------------------------------------------
    void RenderEngine::ShutdownGraphicsContext() {
        s_render_driver_context->GetDriver()->Shutdown();

        s_render_driver_context->Shutdown();
        delete s_render_driver_context;
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Initialize(Window *window) {
        InitializeGraphicsContext(window);

        RenderThreadSynchronization::NotifyRenderReady();
        RenderThreadSynchronization::WaitForMainReady();
        RenderThreadSynchronization::NotifyRenderDone();
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Loop(void *parameter) {
        RT_Initialize(static_cast<Window *>(parameter));
        
        HYP_PROFILE_THREAD("Render Thread");
        while (true) {
            if (s_render_thread_should_exit) {
                break;
            }

            s_render_driver_context->SetVSyncMode(s_vsync_mode);

            {
                HYP_PROFILE_CATEGORY("RenderEngine.WaitForSwapDone", ProfileCategory::Wait);
                RenderThreadSynchronization::WaitForSwapDone();
            }
            
            RenderDriver();

            {
                HYP_PROFILE_SCOPE("RenderEngine.Present");
                s_render_driver_context->SwapBuffers();
                RenderThreadSynchronization::NotifyRenderDone();
            }
        }

        RT_Shutdown();
    }

    //--------------------------------------------------------------
    void RenderEngine::RT_Shutdown() {
        ShutdownGraphicsContext();
    }

}