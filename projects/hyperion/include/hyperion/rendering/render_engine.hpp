#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/rendering/render_command.hpp"

namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class IRenderPipeline;
        class GraphicsContext;
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
        static void Render();
        static void Shutdown();

        static void InitRenderThread(Window *window);
        static void RenderThreadLoop(void *parameter);
        static void ShutdownRenderThread();
        static void InitGraphicsContext(Window *window);
        static void SwapBufferedState();
    private: 
        inline static RenderSettings s_render_settings;

        // If using multiple threads these grouped variables are owned by the Render Thread.
        inline static GraphicsContext *s_graphics_context;
        inline static IRenderPipeline *s_render_pipeline;

        inline static u64 s_render_frame = 0;

        inline static Threading::Thread s_render_thread;
        inline static bool s_exit_requested;
        inline static Queue<RenderCommand> s_update_queue;
        inline static Queue<RenderCommand> s_render_queue;
    private:
        friend class Hyperion::Engine;
    };

}