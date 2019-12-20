#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    struct OpenGLContextLimits {
        s32 max_msaa_samples;
        s32 max_texture_units;
    };

    class OpenGLGraphicsContext : public GraphicsContext {
    private:
        Vector<String> m_extensions;
        OpenGLContextLimits m_limits;
    public:
        inline const Vector<String> &GetExtensions() const { return m_extensions; }
        inline const OpenGLContextLimits &GetLimits() const { return m_limits; }
    protected:
        void Init() override;
    private:
        void CheckExtensions();
        void CheckExtension(const String &extension);

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    };

}