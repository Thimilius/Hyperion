#include "hyppch.hpp"

#include "hyperion/rendering/render_command.hpp"

#include "hyperion/driver/opengl/opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void RenderCommand::Init(RenderBackend backend_api) {
        switch (backend_api) {
            case RenderBackend::None: {
                HYP_PANIC_MESSAGE("Rendering", "Trying to create renderer with no backend API!");
                break;
            }
            case RenderBackend::OpenGL: {
                s_render_api.reset(new OpenGLRenderAPI());
                s_render_api->s_render_backend = backend_api;
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}