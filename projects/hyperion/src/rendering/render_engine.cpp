#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/rendering/render_command.hpp"
#include "hyperion/rendering/forward_render_pipeline.hpp"
#include "hyperion/driver/opengl/opengl_render_driver.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::Init(const RenderSettings &settings) {
        s_render_backend = settings.backend;

        switch (settings.backend) {
            case RenderBackend::OpenGL: {
                RenderCommand::s_render_driver.reset(new OpenGLRenderDriver());
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        switch (settings.path) {
            case Rendering::RenderPath::Forward: {
                s_render_pipeline.reset(new Rendering::ForwardRenderPipeline());
                break;
            }
            case Rendering::RenderPath::Custom: {
                HYP_ASSERT_MESSAGE(settings.custom_pipeline, "When using a custom render path, a custom render pipeline must be provided!");
                s_render_pipeline.reset(settings.custom_pipeline);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    void RenderEngine::Render() {
        s_render_pipeline->Render();
    }

    void RenderEngine::Shutdown() {

    }

}