//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/driver/opengl/opengl_render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct OpenGLRenderContextLimits {
    uint32 max_texture_units;
    uint32 max_texture_size;

    uint32 max_msaa_samples;

    uint32 max_framebuffer_color_attachments;
    uint32 max_framebuffer_width;
    uint32 max_framebuffer_height;
  };

  class OpenGLRenderContext : public IRenderContext {
  public:
    virtual ~OpenGLRenderContext() override = default;
  public:
    inline RenderContextProperties GetProperties() const override { return m_properties; }

    inline IRenderDriver *GetDriver() { return &m_render_driver; }

    inline const OpenGLRenderContextLimits &GetLimits() const { return m_limits; }
    inline const Array<String> &GetExtensions() const { return m_extensions; }

    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
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
    RenderContextProperties m_properties;
    OpenGLRenderContextLimits m_limits;
    Array<String> m_extensions;

    OpenGLRenderDriver m_render_driver;
  };

}