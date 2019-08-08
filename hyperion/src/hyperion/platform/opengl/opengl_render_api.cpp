#include "hyppch.hpp"

#include "opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void COpenGLRenderAPI::SetClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void COpenGLRenderAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void COpenGLRenderAPI::SetViewport(s32 x, s32 y, s32 width, s32 height) {
        glViewport(x, y, width, height);
    }

}