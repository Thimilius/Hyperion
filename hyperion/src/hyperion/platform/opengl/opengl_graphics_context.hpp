#pragma once

#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    class COpenGLGraphicsContext : public CGraphicsContext {
    public:
        void Init() override;
    private:
        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    };

}