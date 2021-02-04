#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/threading/render_thread_commands.hpp"
#include "hyperion/rendering/threading/render_thread_command_queue.hpp"

namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class GraphicsContext;
        class IRenderPipeline;
        class RenderThreadCommandExecutor;
        class RenderThreadRenderDriver;
    }
}

namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static RenderBackend GetBackend() { return s_render_settings.backend; }
        inline static IRenderDriver *GetRenderDriver() { return s_render_driver; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void PreInitialize(const RenderSettings &settings, Window *window);
        static void Initialize();
        static void Render();
        static void PreShutdown();
        static void Shutdown();
        static void Exit();

        static void InitGraphicsContextAndBackend(Window *window);
        static void SwapBuffers();

        static void InitRenderThread(Window *window);
        static void RenderThreadLoop(void *parameter);
        static void ShutdownRenderThread();

        inline static RenderThreadCommandQueue<RenderThreadCommandType> &GetCommandQueue() { return s_update_queue; }
        static void ExecuteRenderCommands();
        inline static RenderThreadCommandQueue<ImmediateRenderThreadCommandType> &GetImmediateCommandQueue() { return s_immediate_queue; }
        static void ExecutePotentialImmediateRenderCommand();
    private: 
        inline static RenderSettings s_render_settings;
        inline static IRenderPipeline *s_render_pipeline;
        inline static uint64 s_render_frame = 0;

        inline static GraphicsContext *s_graphics_context;

        inline static IRenderDriver *s_render_driver;
        inline static IRenderDriver *s_render_driver_backend;
        inline static IRenderDriver *s_render_driver_wrapper;

        inline static bool s_exit_requested; // TODO: Maybe we can just use an atomic bool in Engine?
        inline static Threading::Thread s_render_thread;
        inline static RenderThreadCommandQueue<RenderThreadCommandType> s_update_queue;
        inline static RenderThreadCommandQueue<RenderThreadCommandType> s_render_queue;

        inline static RenderThreadCommandQueue<ImmediateRenderThreadCommandType> s_immediate_queue;
        inline static std::atomic<bool> s_current_immediate_command_pending;
        inline static ImmediateRenderThreadCommandType s_current_immediate_command_type;
        inline static void *s_current_immediate_command;
        inline static uint64 s_current_immediate_command_size;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderThreadRenderDriver;
    };

}