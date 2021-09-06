#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/view/render_view.hpp"

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

        static RenderView *GetMainRenderView() { return s_render_view; }

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

        inline static RenderView s_first_view;
        inline static RenderView s_second_view;
        inline static RenderView *s_main_view;
        inline static RenderView *s_render_view;

        inline static IRenderPipeline *s_render_pipeline;
        inline static Graphics::GraphicsContext *s_graphics_context;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::CameraSystem;
    };

}