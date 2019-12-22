#include "hyppch.hpp"

#include "hyperion/rendering/render_command.hpp"

#include "hyperion/driver/opengl/opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void RenderCommand::Init(RenderBackend backend) {
        switch (backend) {
            case RenderBackend::OpenGL: {
                s_render_api.reset(new OpenGLRenderAPI());
                s_render_api->s_render_backend = backend;
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}