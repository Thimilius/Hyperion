#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class OpenGLGraphicsDevice;
    class OpenGLGraphicsDeviceContext;
    class OpenGLGraphicsSwapChain;
}

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

    class OpenGLGraphicsContext : public GraphicsContext {
    public:
        virtual ~OpenGLGraphicsContext() override = default;
    public:
        inline GraphicsContextProperties GetProperties() const override { return m_properties; }

        inline const OpenGLGraphicsContextLimits &GetLimits() const { return m_limits; }
        inline const Array<String> &GetExtensions() const { return m_extensions; }

        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) override;
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
        OpenGLGraphicsDevice *m_device;
        OpenGLGraphicsDeviceContext *m_device_context;
        OpenGLGraphicsSwapChain *m_swap_chain;

        GraphicsContextProperties m_properties;
        OpenGLGraphicsContextLimits m_limits;
        Array<String> m_extensions;
    };

}