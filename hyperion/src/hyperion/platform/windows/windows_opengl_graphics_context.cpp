#include "hyppch.hpp"

#include "windows_opengl_graphics_context.hpp"

namespace Hyperion::Rendering {

    CWindowsOpenGLGraphicsContext::CWindowsOpenGLGraphicsContext(HWND window_handle) {
        m_device_context = GetDC(window_handle);
    }

    void CWindowsOpenGLGraphicsContext::Init() {
        // Create temp context for extension loading
        {
            PIXELFORMATDESCRIPTOR pixel_format_descriptor = { 0 };
            pixel_format_descriptor.nSize = sizeof(pixel_format_descriptor);
            pixel_format_descriptor.nVersion = 1;
            pixel_format_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pixel_format_descriptor.iPixelType = PFD_TYPE_RGBA;
            pixel_format_descriptor.cColorBits = 32;
            pixel_format_descriptor.cDepthBits = 24;
            pixel_format_descriptor.cStencilBits = 8;

            s32 pixel_format = ChoosePixelFormat(m_device_context, &pixel_format_descriptor);
            SetPixelFormat(m_device_context, pixel_format, &pixel_format_descriptor);

            HGLRC temp_context = wglCreateContext(m_device_context);
            wglMakeCurrent(m_device_context, temp_context);

            GLADloadproc extension_loader = (GLADloadproc)wglGetProcAddress;
            if (!gladLoadWGLLoader(extension_loader, m_device_context)) {
                HYP_CORE_ERROR("[OpenGL] - Failed to load windows opengl extensions!");
            }

            wglMakeCurrent(m_device_context, NULL);
            wglDeleteContext(temp_context);
        }

        // Create real context
        {
            const s32 pixel_attributes[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 32,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                0
            };
            s32 pixel_format;
            u32 formats_count;
            wglChoosePixelFormatARB(m_device_context, pixel_attributes, NULL, 1, &pixel_format, &formats_count);

            PIXELFORMATDESCRIPTOR pixel_format_descriptor;
            DescribePixelFormat(m_device_context, pixel_format, sizeof(pixel_format_descriptor), &pixel_format_descriptor);
            SetPixelFormat(m_device_context, pixel_format, &pixel_format_descriptor);

            const s32 context_attributes[] = {
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0
            };
            HGLRC opengl_context = wglCreateContextAttribsARB(m_device_context, NULL, context_attributes);
            wglMakeCurrent(m_device_context, opengl_context);

            if (!gladLoadGL()) {
                HYP_CORE_ERROR("[OpenGL] - Failed to load opengl extensions!");
            }
        }

        COpenGLGraphicsContext::Init();
    }

    void CWindowsOpenGLGraphicsContext::SwapBuffers() {
        ::SwapBuffers(m_device_context);
    }

    void CWindowsOpenGLGraphicsContext::SetVSyncMode(EVSyncMode vsync_mode) {
        s32 swap_interval = 0;
        switch (vsync_mode) {
            case EVSyncMode::DontSync: swap_interval = 0; break;
            case EVSyncMode::EveryVBlank: swap_interval = 1; break;
            case EVSyncMode::EverySecondVBlank: swap_interval = 2; break;
            default: HYP_ASSERT(false); break;
        }
        wglSwapIntervalEXT(swap_interval);
    }

}