#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class CameraSystem;
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

        static RenderFrame *GetMainRenderFrame() { return s_main_frame; }

        static void RT_Initialize(Window *window);
        static void RT_Loop(void *parameter);
        static void RT_Shutdown();

        static void InitializeGraphicsContext(Window *window);
        static void ShutdownGraphicsContext();

        static void SynchronizeMainAndRenderThread();
    private: 
        inline static RenderSettings s_render_settings;
        
        inline static Threading::Thread s_render_thread;
        inline static std::atomic<bool8> s_render_thread_should_exit;

        inline static RenderFrame s_first_frame;
        inline static RenderFrame s_second_frame;
        inline static RenderFrame *s_main_frame;
        inline static RenderFrame *s_render_frame;

        inline static IRenderPipeline *s_render_pipeline;
        inline static Graphics::GraphicsContext *s_graphics_context;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::CameraSystem;
    };

}