#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/render_driver_context.hpp"
#include "hyperion/render/driver/opengl/opengl_render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLRenderDriverContextLimits {
        uint32 max_texture_units;
        uint32 max_texture_size;

        uint32 max_msaa_samples;

        uint32 max_framebuffer_color_attachments;
        uint32 max_framebuffer_width;
        uint32 max_framebuffer_height;
    };

    class OpenGLRenderDriverContext : public IRenderDriverContext {
    public:
        virtual ~OpenGLRenderDriverContext() override = default;
    public:
        inline RenderDriverContextProperties GetProperties() const override { return m_properties; }

        inline IRenderDriver *GetDriver() { return &m_render_driver; }

        inline const OpenGLRenderDriverContextLimits &GetLimits() const { return m_limits; }
        inline const Array<String> &GetExtensions() const { return m_extensions; }

        void Initialize(const RenderDriverContextDescriptor &descriptor) override;
        void Shutdown() override;
    private:
        void InitializeDebug();

        void QueryExtensions();
        void QueryProperties();
        void QueryLimits();

        void CheckExtensions();
        void CheckExtension(const String &extension);
    private:
        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    private:
        RenderDriverContextProperties m_properties;
        OpenGLRenderDriverContextLimits m_limits;
        Array<String> m_extensions;

        OpenGLRenderDriver m_render_driver;
    };

}