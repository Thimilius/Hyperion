#include "hyppch.hpp"

#include "hyperion/rendering/render_command.hpp"

#include "hyperion/driver/opengl/opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void RenderCommand::Init() {
        switch (RenderAPI::GetBackendAPI()) {
            case RenderBackendAPI::OpenGL: {
                s_render_api = new OpenGLRenderAPI();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}