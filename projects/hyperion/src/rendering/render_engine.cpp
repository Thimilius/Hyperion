#include "hyppch.hpp"

#include "hyperion/rendering/render_engine.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

namespace Hyperion::Rendering {

    void RenderEngine::Init(RenderBackend backend) {
        switch (backend) {
            case RenderBackend::OpenGL: {
                s_render_driver.reset(new OpenGLRenderDriver());
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        s_render_backend = backend;
        s_render_driver->Init();
    }

    void RenderEngine::Shutdown() {
        s_render_driver->Shutdown();
    }

}