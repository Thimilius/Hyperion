#pragma once

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"

#include <Windows.h>

namespace Hyperion::Rendering {

    class WindowsOpenGLGraphicsContext : public OpenGLGraphicsContext {
    private:
        HDC m_device_context;
    public:
        WindowsOpenGLGraphicsContext(HWND window_handle);
    private:
        void Init() override;
        void SwapBuffers() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;
    };

}