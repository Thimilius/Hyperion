#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class IRenderPipeline;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static Graphics::GraphicsBackend GetBackend() { return s_render_settings.graphics_backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;
    private:
        static void PreInitialize(const RenderSettings &settings, Window *window);
        static void Initialize();
        static void Render();
        static void Shutdown();

        static void RequestExit();

        static void RT_Initialize(Window *window);
        static void RT_Loop(void *parameter);
        static void RT_Shutdown();

        static void InitializeGraphicsContext(Window *window);
        static void ShutdownGraphicsContext();

        static void SynchronizeMainAndRenderThread();
    private: 
        inline static RenderSettings s_render_settings;
        
        inline static Threading::Thread s_render_thread;
        inline static std::atomic<bool> s_render_thread_should_exit;

        inline static IRenderPipeline *s_render_pipeline;

        inline static Graphics::GraphicsContext *s_graphics_context;
        inline static Graphics::GraphicsDevice *s_graphics_device;
        inline static Graphics::GraphicsDeviceContext *s_graphics_device_context;
        inline static Graphics::GraphicsSwapChain *s_graphics_swap_chain;
    private:
        friend class Hyperion::Engine;
    };

}