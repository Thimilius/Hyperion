#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::PreInit(const RenderSettings &settings) {
        s_render_backend = settings.backend;

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
    }

    void RenderEngine::Shutdown() {
        //s_render_pipeline->Shutdown();
        delete s_render_pipeline;
    }

}