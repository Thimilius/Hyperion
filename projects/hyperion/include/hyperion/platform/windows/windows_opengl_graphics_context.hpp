#pragma once

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"

#include <Windows.h>

namespace Hyperion::Rendering {

    class WindowsOpenGLGraphicsContext : public OpenGLGraphicsContext {
    public:
        WindowsOpenGLGraphicsContext(HWND window_handle);
        ~WindowsOpenGLGraphicsContext() override;
    private:
        void Init() override;
        void Present() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;
    private:
        HDC m_device_context;
        HGLRC m_opengl_context;
    };

}