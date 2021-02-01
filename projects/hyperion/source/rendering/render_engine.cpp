#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/core/threading/synchronization.hpp"
#include "hyperion/driver/opengl/opengl_render_driver.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/rendering/graphics_context.hpp"
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"
#include "hyperion/rendering/threading/render_thread_commands.hpp"
#include "hyperion/rendering/threading/render_thread_command_executor.hpp"
#include "hyperion/rendering/threading/render_thread_render_driver.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::PreInitialize(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;
        
        switch (s_render_settings.threading_mode) {
            case RenderThreadingMode::SingleThreaded: {
                InitGraphicsContextAndBackend(window);
                break;
            }
            case RenderThreadingMode::MultiThreaded: {
                s_render_thread.Start(RenderThreadLoop, window);
                s_render_thread.SetName("Render Thread");
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        switch (s_render_settings.pipeline) {
            case RenderPipeline::Forward: s_render_pipeline = new ForwardRenderPipeline(); break;
            case RenderPipeline::Custom: s_render_pipeline = s_render_settings.custom_pipeline; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::Initialize() {
        s_render_pipeline->Initialize(s_render_driver);
    }

    void RenderEngine::Render() {
        RenderPipelineContext render_pipeline_context;
        Camera *camera = WorldManager::GetActiveWorld()->FindComponentOfType<Camera>();
        render_pipeline_context.m_camera_data = camera->GetCameraData();
        s_render_pipeline->Render(s_render_driver, render_pipeline_context);

        // The following block ends a frame on the Main Thread:
        {
            if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
                s_graphics_context->Present();
            } else {
                Synchronization::WaitForRenderDone();
                SwapBuffers();
                Synchronization::NotifySwapDone();
            }

            s_render_frame++;
        }
    }

    void RenderEngine::PreShutdown() {
        s_render_pipeline->Shutdown(s_render_driver);
    }

    void RenderEngine::Shutdown() {
        delete s_render_pipeline;
        delete s_render_driver_wrapper;

        if (s_render_settings.threading_mode == RenderThreadingMode::MultiThreaded) {
            // To properly destroy all resources, we have to send all render thread commands to the Render Thread one last time
            // and wait for them to execute, so that the Render Thread is finally shut down.
            SwapBuffers();
            Synchronization::NotifySwapDone();
            s_render_thread.Join();
        } 

        if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
            delete s_render_driver_backend;
            delete s_graphics_context;
        }
    }

    void RenderEngine::Exit() {
        GetCommandQueue().Allocate(RenderThreadCommandType::Exit);
    }

    void RenderEngine::InitGraphicsContextAndBackend(Window *window) {
        // The graphics context is the very first thing we need to initialize so that resources can be created properly.
        s_graphics_context = window->CreateGraphicsContext(s_render_settings.backend);
        s_graphics_context->Initialize(GraphicsContextDescriptor());
        s_graphics_context->SetVSyncMode(VSyncMode::DontSync);

        switch (s_render_settings.backend) {
            case RenderBackend::OpenGL: {
                s_render_driver_backend = new OpenGLRenderDriver();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        s_render_driver_backend->Initialize(s_graphics_context);

        if (s_render_settings.threading_mode == RenderThreadingMode::MultiThreaded) {
            s_render_driver_wrapper = new RenderThreadRenderDriver();
            s_render_driver = s_render_driver_wrapper;
        } else {
            s_render_driver = s_render_driver_backend;
        }
    }

    void RenderEngine::SwapBuffers() {
        s_render_queue.Clear();
        std::swap(s_update_queue, s_render_queue);
    }

    void RenderEngine::InitRenderThread(Window *window) {
        InitGraphicsContextAndBackend(window);

        Synchronization::NotifyRenderReady();
        Synchronization::WaitForUpdateReady();
    }

    void RenderEngine::RenderThreadLoop(void *parameter) {
        InitRenderThread(static_cast<Window *>(parameter));

        while (true) {
            ExecuteRenderCommands();

            if (s_exit_requested) {
                break;
            }

            s_graphics_context->Present();
            Synchronization::NotifyRenderDone();

            // While we are waiting for the main thread to give us the new set of commands,
            // we want to execute all potentially incoming immediate render thread commands.
            while (true) {
                ExecutePotentialImmediateRenderCommand();
            
                if (Synchronization::WaitUnblockedForSwapDone()) {
                    break;
                }
            }
        }

        ShutdownRenderThread();
    }

    void RenderEngine::ShutdownRenderThread() {
        // We have to wait for all last render commands to properly destroy all GPU resources.
        Synchronization::WaitForSwapDone();
        ExecuteRenderCommands();

        delete s_render_driver_backend;
        delete s_graphics_context;
    }

    void RenderEngine::ExecuteRenderCommands() {
        // Execute render thread commands like a virtual machine...
        auto program_counter = s_render_queue.GetData();
        auto program_counter_end = s_render_queue.GetData() + s_render_queue.GetSize();
        RenderThreadCommandType command_type;
        while (program_counter < program_counter_end) {
            ExecutePotentialImmediateRenderCommand();

            command_type = *reinterpret_cast<RenderThreadCommandType *>(program_counter);
            program_counter += sizeof(RenderThreadCommandType);

            if (command_type == RenderThreadCommandType::Exit) {
                s_exit_requested = true;
            } else {
                program_counter += static_cast<size_t>(RenderThreadCommandExecutor::Execute(s_render_driver_backend, command_type, program_counter));
            }
        }
    }

    void RenderEngine::ExecutePotentialImmediateRenderCommand() {
        if (s_is_immediate_render_command_pending) {
            s_is_immediate_render_command_pending = false;
            RenderThreadCommandExecutor::ExecuteImmediate(s_render_driver_backend, s_immediate_render_command);
            Synchronization::NotifyImmediateCommandDone();
        }
    }

}