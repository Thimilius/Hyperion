#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::PreInit(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;

        if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
            InitGraphicsContext(window);
        } else {
            s_render_thread.Start(RenderThreadLoop, window);
            s_render_thread.SetName("Render Thread");
        }
    }

    void RenderEngine::Render() {
        // Depending on render threading mode either create and enqueue render commands or just execute them directly...

        // The following block ends a frame on the Main Thread:
        {
            if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
                // Only in single threaded mode are we swaping the buffers here on the Main Thread.
                s_graphics_context->SwapBuffers();
            } else {
                EngineSync::WaitForRenderDone();
                SwapBufferedState();
                EngineSync::NotifySwapDone();
            }

            s_render_frame++;
        }
    }

    void RenderEngine::Shutdown() {
        if (s_render_settings.threading_mode == RenderThreadingMode::MultiThreaded) {
            s_render_thread.Join();
        }
    }

    void RenderEngine::InitRenderThread(Window *window) {
        InitGraphicsContext(window);

        EngineSync::NotifyRenderReady();
        EngineSync::WaitForUpdateReady();
    }

    void RenderEngine::RenderThreadLoop(void *parameter) {
        InitRenderThread(static_cast<Window *>(parameter));

        while (true) {
            // Execute render commands...
            Queue<RenderCommand> &queue = s_render_queue;
            while (!s_exit_requested && !queue.empty()) {
                RenderCommand command = queue.front();
                queue.pop();

                s_exit_requested = command == RenderCommand::Exit;
                if (s_exit_requested) {
                    break;
                }
            }

            if (s_exit_requested) {
                break;
            }

            s_graphics_context->SwapBuffers();

            EngineSync::NotifyRenderDone();
            EngineSync::WaitForSwapDone();
        }

        ShutdownRenderThread();
    }

    void RenderEngine::ShutdownRenderThread() {
        delete s_render_pipeline;
        delete s_graphics_context;
    }

    void RenderEngine::InitGraphicsContext(Window *window) {
        // The graphics context is the very first thing we need to initialize so that resources can be created properly.
        s_graphics_context = window->CreateGraphicsContext(s_render_settings.backend);
        s_graphics_context->Init();
        s_graphics_context->SetVSyncMode(VSyncMode::DontSync);

        switch (s_render_settings.backend) {
            case RenderBackend::OpenGL: {
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::SwapBufferedState() {
        std::swap(s_update_queue, s_render_queue);
    }

}