#pragma once

#include "hyperion/core/app/application_settings.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion::Rendering {

    class RenderEngine {
    public:
        inline static RenderBackend GetBackend() { return s_render_backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void PreInit(const RenderSettings &settings);
        static void Init(const RenderSettings &settings);
        static void Render();
        static void Shutdown();
    private: 
        inline static RenderBackend s_render_backend;

        inline static Scope<Rendering::RenderPipeline> s_render_pipeline;
    private:
        friend class Hyperion::Engine;
    };

}