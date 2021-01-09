#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/rendering/render_command.hpp"

namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class GraphicsContext;
        class IRenderPipeline;
        class MultithreadedRenderDriver;
    }
}

namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static RenderBackend GetBackend() { return s_render_settings.backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void PreInit(const RenderSettings &settings, Window *window);
        static void Init();
        static void Render();
        static void Shutdown();
        static void Exit();

        static void InitRenderThread(Window *window);
        static void RenderThreadLoop(void *parameter);
        static void ExecuteRenderCommand(const RenderCommand &render_command);
        static void ShutdownRenderThread();
        static void InitGraphicsContext(Window *window);
        static void SwapBufferedState();
    private: 
        inline static RenderSettings s_render_settings;
        inline static IRenderPipeline *s_render_pipeline;
        inline static u64 s_render_frame = 0;

        // If using multiple threads these grouped variables are owned by the Render Thread.
        inline static GraphicsContext *s_graphics_context;
        inline static IRenderDriver *s_render_driver_backend;
        inline static IRenderDriver *s_render_driver_multithreaded;

        inline static Threading::Thread s_render_thread;
        inline static bool s_exit_requested;
        // TODO: Abstract into RenderCommandQueue
        inline static Queue<RenderCommand> s_update_queue;
        inline static Queue<RenderCommand> s_render_queue;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::MultithreadedRenderDriver;
    };

}