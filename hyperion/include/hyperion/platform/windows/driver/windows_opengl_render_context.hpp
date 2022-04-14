//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/opengl/opengl_render_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class WindowsOpenGLRenderContext : public OpenGLRenderContext {
  public:
    WindowsOpenGLRenderContext(HDC helper_device_context);
  public:
    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;

    void MakeCurrent(Window *window) override;
    void SwapBuffers(Window *window) override;
    void SetVSyncMode(VSyncMode vsync_mode) override;
  private:
    void LoadOpenGLExtensions(HDC helper_device_context);
  private:
    HGLRC m_opengl_context;
  };

}
