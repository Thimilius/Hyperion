#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::PreInit(const RenderSettings &settings, GraphicsContext *graphics_context) {
        s_render_settings = settings;
        s_graphics_context = graphics_context;

        // The graphics context is the very first thing we need to initialize so that resources can be created properly.
        s_graphics_context->Init();

        switch (settings.backend) {
            case RenderBackend::OpenGL: {
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::Init(const RenderSettings &settings) {
        //s_render_pipeline->Init();
    }

    void RenderEngine::Render() {
        //s_render_pipeline->Render();

        // Only in single threaded mode are we swaping the buffers here on the main thread.
        if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
            s_graphics_context->SwapBuffers();
        }
    }

    void RenderEngine::Shutdown() {
        //s_render_pipeline->Shutdown();
        delete s_render_pipeline;
        delete s_graphics_context;
    }

}