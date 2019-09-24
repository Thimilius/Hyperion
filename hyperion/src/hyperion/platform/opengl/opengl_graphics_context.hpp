#pragma once

#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    struct SOpenGLContextLimits {
        s32 max_msaa_samples;
        s32 max_texture_units;
    };

    class COpenGLGraphicsContext : public CGraphicsContext {
    private:
        SOpenGLContextLimits m_limits;
    public:
        inline const SOpenGLContextLimits &GetLimits() const { return m_limits; }
    protected:
        void Init() override;

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    };

}