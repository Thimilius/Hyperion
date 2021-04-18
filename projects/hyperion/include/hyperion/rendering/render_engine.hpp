#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/threading/thread.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class GraphicsContext;
        class IRenderPipeline;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static RenderBackend GetBackend() { return s_render_settings.backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;
    private:
        static void PreInitialize(const RenderSettings &settings, Window *window);
        static void Initialize();
        static void Render();
        static void PreShutdown();
        static void Shutdown();

        static void InitGraphicsContextAndBackend(Window *window);
    private: 
        inline static RenderSettings s_render_settings;
        inline static IRenderPipeline *s_render_pipeline;
        inline static GraphicsContext *s_graphics_context;
    private:
        friend class Hyperion::Engine;
    };

}