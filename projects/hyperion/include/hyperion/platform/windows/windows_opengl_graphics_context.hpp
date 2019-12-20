#pragma once

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"

#include <Windows.h>

namespace Hyperion::Rendering {

    class CWindowsOpenGLGraphicsContext : public COpenGLGraphicsContext {
    private:
        HDC m_device_context;
    public:
        CWindowsOpenGLGraphicsContext(HWND window_handle);
    private:
        void Init() override;
        void SwapBuffers() override;
        void SetVSyncMode(EVSyncMode vsync_mode) override;
    };

}