#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <atomic>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/rendering/driver/render_driver_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;

    namespace Rendering {
        class IRenderPipeline;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static RenderBackend GetBackend() { return s_render_settings.backend; }
        inline static RenderFrame *GetMainRenderFrame() { return s_main_frame; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;
    private:
        static void PreInitialize(const RenderSettings &settings, Window *window);
        static void Initialize();
        static void Render();
        static void Shutdown();

        static void RequestExit();

        static void InitializeGraphicsContext(Window *window);
        static void ShutdownGraphicsContext();

        static void SwapRenderFrames();

        static void RT_Initialize(Window *window);
        static void RT_Loop(void *parameter);
        static void RT_Shutdown();
    private: 
        inline static RenderSettings s_render_settings;
        
        inline static Threading::Thread s_render_thread;
        inline static std::atomic<bool8> s_render_thread_should_exit;

        inline static RenderFrame s_first_frame;
        inline static RenderFrame s_second_frame;
        inline static RenderFrame *s_main_frame;
        inline static RenderFrame *s_render_frame;

        inline static IRenderPipeline *s_render_pipeline;
        inline static IRenderDriverContext *s_render_driver_context;
    private:
        friend class Hyperion::Engine;
    };

}