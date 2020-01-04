#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/graphics_context.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    class OpenGLGraphicsContext : public GraphicsContext {
    private:
        Vector<String> m_extensions;
    public:
        virtual ~OpenGLGraphicsContext() override = default;

        inline const Vector<String> &GetExtensions() const { return m_extensions; }
    protected:
        void Init() override;
    private:
        void CheckExtensions();
        void CheckExtension(const String &extension);

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    };

}