#pragma once

#include "hyperion/core/app/application_settings.hpp"

namespace Hyperion {
    class Engine;

    namespace Rendering {
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
        static void PreInit(const RenderSettings &settings, GraphicsContext *graphics_context);
        static void Init(const RenderSettings &settings);
        static void Render();
        static void Shutdown();
    private: 
        inline static RenderSettings s_render_settings;

        inline static Rendering::GraphicsContext *s_graphics_context;
        inline static Rendering::IRenderPipeline *s_render_pipeline;
    private:
        friend class Hyperion::Engine;
    };

}