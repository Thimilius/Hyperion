#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct OpenGLGraphicsContextLimits {
        uint32 max_texture_units;
        uint32 max_texture_size;

        uint32 max_msaa_samples;

        uint32 max_framebuffer_color_attachments;
        uint32 max_framebuffer_width;
        uint32 max_framebuffer_height;
    };

    class OpenGLGraphicsContext : public IGraphicsContext {
    public:
        virtual ~OpenGLGraphicsContext() override = default;
    public:
        inline GraphicsContextProperties GetProperties() const override { return m_properties; }

        inline const OpenGLGraphicsContextLimits &GetLimits() const { return m_limits; }
        inline const Vector<String> &GetExtensions() const { return m_extensions; }

        void Initialize(const GraphicsContextDescriptor &descriptor) override;

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) override;
    private:
        void InitDebug();

        void QueryExtensions();
        void QueryProperties();
        void QueryLimits();

        void CheckExtensions();
        void CheckExtension(const String &extension);
    private:
        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    private:
        GraphicsContextProperties m_properties;
        OpenGLGraphicsContextLimits m_limits;
        Vector<String> m_extensions;
    };

}