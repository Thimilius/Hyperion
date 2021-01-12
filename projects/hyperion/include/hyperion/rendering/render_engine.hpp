#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_command_queue.hpp"

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
        inline static IRenderDriver *GetRenderDriver() { return s_render_driver; }
        inline static RenderCommandQueue &GetCommandQueue() { return s_update_queue; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void PreInit(const RenderSettings &settings, Window *window);
        static void Init();
        static void Render();
        static void Shutdown();
        static void Exit();

        static void InitGraphicsContextAndBackend(Window *window);
        static void SwapBuffers();

        static void InitRenderThread(Window *window);
        static void RenderThreadLoop(void *parameter);
        static void ShutdownRenderThread();

        static u64 ExecuteRenderCommand(RenderCommandType command_type, const void *data);
    private: 
        inline static RenderSettings s_render_settings;
        inline static IRenderPipeline *s_render_pipeline;
        inline static u64 s_render_frame = 0;

        inline static GraphicsContext *s_graphics_context;

        inline static IRenderDriver *s_render_driver;
        inline static IRenderDriver *s_render_driver_backend;
        inline static IRenderDriver *s_render_driver_wrapper;

        inline static Threading::Thread s_render_thread;
        inline static bool s_exit_requested;
        inline static RenderCommandQueue s_update_queue;
        inline static RenderCommandQueue s_render_queue;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::MultithreadedRenderDriver;
    };

}