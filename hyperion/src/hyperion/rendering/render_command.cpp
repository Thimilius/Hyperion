#include "hyppch.hpp"

#include "hyperion/rendering/render_command.hpp"

#include "hyperion/platform/opengl/opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void CRenderCommand::Init() {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: {
                s_render_api = new COpenGLRenderAPI();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE;
        }
    }

}