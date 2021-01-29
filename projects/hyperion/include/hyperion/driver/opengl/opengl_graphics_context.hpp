#pragma once

#include <glad/glad.h>

#include "hyperion/common.hpp"
#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    class OpenGLGraphicsContext : public GraphicsContext {
    public:
        virtual ~OpenGLGraphicsContext() override = default;

        inline const Vector<String> &GetExtensions() const { return m_extensions; }
    protected:
        void Initialize(const GraphicsContextDescriptor &descriptor) override;
    private:
        void CheckExtensions();
        void CheckExtension(const String &extension);

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    private:
        Vector<String> m_extensions;
    };

}