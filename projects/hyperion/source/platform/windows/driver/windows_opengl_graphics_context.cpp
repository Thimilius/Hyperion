//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/driver/windows_opengl_render_context.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad_wgl.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  WindowsOpenGLRenderContext::WindowsOpenGLRenderContext(HDC device_context, HDC helper_device_context) {
    m_device_context = device_context;

    LoadOpenGLExtensions(helper_device_context);
  }

  //--------------------------------------------------------------
  void WindowsOpenGLRenderContext::Initialize(const RenderContextDescriptor &descriptor) {
    const int32 pixel_attributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, descriptor.color_bits,
            WGL_DEPTH_BITS_ARB, descriptor.depth_bits,
            WGL_STENCIL_BITS_ARB, descriptor.stencil_bits,
            WGL_ACCUM_BITS_ARB, 0,
            WGL_SAMPLE_BUFFERS_ARB, descriptor.msaa_samples > 0 ? 1 : 0,
            WGL_SAMPLES_ARB, descriptor.msaa_samples,
            0
    };
    int32 pixel_format;
    uint32 formats_count;
    wglChoosePixelFormatARB(m_device_context, pixel_attributes, nullptr, 1, &pixel_format, &formats_count);

    PIXELFORMATDESCRIPTOR pixel_format_descriptor;
    if (!DescribePixelFormat(m_device_context, pixel_format, sizeof(pixel_format_descriptor), &pixel_format_descriptor)) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to describe pixel format for context!");
    }
    if (!SetPixelFormat(m_device_context, pixel_format, &pixel_format_descriptor)) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to set pixel format for context!");
    }

#ifdef HYP_DEBUG
    const int32 context_attributes[] = {
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };
#else
    const int32 context_attributes[] = {
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 6,
        // NOTE: We would like to have the no error context but it does not seem supported by the Intel driver.
        //WGL_CONTEXT_OPENGL_NO_ERROR_ARB, 1,
        0
    };
#endif
    m_opengl_context = wglCreateContextAttribsARB(m_device_context, nullptr, context_attributes);
    if (!m_opengl_context) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to create context!");
    }

    wglMakeCurrent(m_device_context, m_opengl_context);

    if (!gladLoadGL()) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to load extensions!");
    }

    OpenGLRenderContext::Initialize(descriptor);
  }

  //--------------------------------------------------------------
  void WindowsOpenGLRenderContext::Shutdown() {
    OpenGLRenderContext::Shutdown();

    wglDeleteContext(m_opengl_context);
  }

  //--------------------------------------------------------------
  void WindowsOpenGLRenderContext::SwapBuffers() {
    ::SwapBuffers(m_device_context);
  }

  //--------------------------------------------------------------
  void WindowsOpenGLRenderContext::SetVSyncMode(VSyncMode vsync_mode) {
    int32 swap_interval = 0;
    switch (vsync_mode) {
      case VSyncMode::DontSync: swap_interval = 0; break;
      case VSyncMode::EveryVBlank: swap_interval = 1; break;
      case VSyncMode::EverySecondVBlank: swap_interval = 2; break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
    wglSwapIntervalEXT(swap_interval);
  }

  //--------------------------------------------------------------
  void WindowsOpenGLRenderContext::LoadOpenGLExtensions(HDC helper_device_context) {
    PIXELFORMATDESCRIPTOR pixel_format_descriptor = { 0 };
    pixel_format_descriptor.nSize = sizeof(pixel_format_descriptor);
    pixel_format_descriptor.nVersion = 1;
    pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
    pixel_format_descriptor.cColorBits = 32;
    pixel_format_descriptor.cDepthBits = 24;
    pixel_format_descriptor.cStencilBits = 8;

    int32 pixel_format = ChoosePixelFormat(helper_device_context, &pixel_format_descriptor);
    if (!SetPixelFormat(helper_device_context, pixel_format, &pixel_format_descriptor)) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to set pixel format for temporary context!");
    }

    HGLRC temp_context = wglCreateContext(helper_device_context);
    if (!temp_context) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to create temporary context!");
    }

    wglMakeCurrent(helper_device_context, temp_context);

    GLADloadproc extension_loader = (GLADloadproc)wglGetProcAddress;
    if (!gladLoadWGLLoader(extension_loader, helper_device_context)) {
      HYP_PANIC_MESSAGE("OpenGL", "Failed to load windows extensions!");
    }

    wglMakeCurrent(helper_device_context, nullptr);
    wglDeleteContext(temp_context);
  }

}