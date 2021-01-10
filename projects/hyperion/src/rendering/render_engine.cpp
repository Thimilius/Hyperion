#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/driver/opengl/opengl_render_driver.hpp"
#include "hyperion/rendering/graphics_context.hpp"
#include "hyperion/rendering/multithreaded_render_driver.hpp"
#include "hyperion/rendering/render_operation.hpp"
#include "hyperion/rendering/pipelines/forward_render_pipeline.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::PreInit(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;
        
        switch (s_render_settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                InitGraphicsContext(window);
                RenderOperation::s_render_driver = s_render_driver_backend;
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                s_render_driver_multithreaded = new MultithreadedRenderDriver();
                RenderOperation::s_render_driver = s_render_driver_multithreaded;

                s_render_thread.Start(RenderThreadLoop, window);
                s_render_thread.SetName("Render Thread");
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        switch (s_render_settings.pipeline) {
            case RenderPipeline::Forward: {
                s_render_pipeline = new ForwardRenderPipeline();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::Init() {
        s_render_pipeline->Init();
    }

    void RenderEngine::Render() {
        // Depending on render threading mode either create and enqueue render commands or just execute them directly...
        s_render_pipeline->Render();

        // The following block ends a frame on the Main Thread:
        {
            if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
                // Only in single threaded mode are we swaping the buffers here on the Main Thread.
                s_graphics_context->SwapBuffers();
            } else {
                Synchronization::WaitForRenderDone();
                SwapBufferedState();
                Synchronization::NotifySwapDone();
            }

            s_render_frame++;
        }
    }

    void RenderEngine::Shutdown() {
        s_render_pipeline->Shutdown();

        delete s_render_pipeline;
        delete s_render_driver_multithreaded;
        delete s_render_driver_backend;

        if (s_render_settings.threading_mode == RenderThreadingMode::MultiThreaded) {
            s_render_thread.Join();
        }
    }

    void RenderEngine::Exit() {
        RenderCommand command = { };
        command.type = RenderCommandType::Exit;
        s_update_queue.push(command);
    }

    void RenderEngine::InitRenderThread(Window *window) {
        InitGraphicsContext(window);

        Synchronization::NotifyRenderReady();
        Synchronization::WaitForUpdateReady();
    }

    void RenderEngine::RenderThreadLoop(void *parameter) {
        InitRenderThread(static_cast<Window *>(parameter));

        while (true) {
            // Execute render commands...
            Queue<RenderCommand> &queue = s_render_queue;
            while (!s_exit_requested && !queue.empty()) {
                RenderCommand command = queue.front();
                queue.pop();

                s_exit_requested = command.type == RenderCommandType::Exit;
                if (s_exit_requested) {
                    break;
                } else {
                    ExecuteRenderCommand(command);
                }
            }

            if (s_exit_requested) {
                break;
            }

            s_graphics_context->SwapBuffers();

            Synchronization::NotifyRenderDone();
            Synchronization::WaitForSwapDone();
        }

        ShutdownRenderThread();
    }

    void RenderEngine::ExecuteRenderCommand(const RenderCommand &render_command) {
        switch (render_command.type) {
            case RenderCommandType::Clear: {
                s_render_driver_backend->Clear(render_command.clear.clear_mask, render_command.clear.color);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::ShutdownRenderThread() {
        delete s_graphics_context;
    }

    void RenderEngine::InitGraphicsContext(Window *window) {
        // The graphics context is the very first thing we need to initialize so that resources can be created properly.
        s_graphics_context = window->CreateGraphicsContext(s_render_settings.backend);
        s_graphics_context->Init();
        s_graphics_context->SetVSyncMode(VSyncMode::DontSync);

        switch (s_render_settings.backend) {
            case RenderBackend::OpenGL: {
                s_render_driver_backend = new OpenGLRenderDriver();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::SwapBufferedState() {
        std::swap(s_update_queue, s_render_queue);
    }

}