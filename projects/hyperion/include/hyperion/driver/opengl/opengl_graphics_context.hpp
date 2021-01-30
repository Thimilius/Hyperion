#pragma once

#include <glad/glad.h>

#include "hyperion/common.hpp"
#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    struct OpenGLGraphicsContextLimits {
        uint32 max_texture_units;
        uint32 max_texture_size;

        uint32 max_msaa_samples;

        uint32 max_framebuffer_color_attachments;
        uint32 max_framebuffer_width;
        uint32 max_framebuffer_height;
    };

    class OpenGLGraphicsContext : public GraphicsContext {
    public:
        virtual ~OpenGLGraphicsContext() override = default;

        inline const Vector<String> &GetExtensions() const { return m_extensions; }
        inline const OpenGLGraphicsContextLimits &GetLimits() const { return m_limits; }
    protected:
        void Initialize(const GraphicsContextDescriptor &descriptor) override;
    private:
        void QueryExtensions();
        void QueryProperties();
        void QueryLimits();

        void CheckExtensions();
        void CheckExtension(const String &extension);

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    private:
        Vector<String> m_extensions;
        OpenGLGraphicsContextLimits m_limits;
    };

}