#pragma once

#include "platform/opengl/opengl_graphics_context.hpp"

#include <Windows.h>

namespace Hyperion::Rendering {

    class CWindowsOpenGLGraphicsContext : public COpenGLGraphicsContext {
    private:
        HDC m_device_context;
    public:
        CWindowsOpenGLGraphicsContext(HWND window_handle);

        void Init() override;
        void SwapBuffers() override;
        void SetVSyncMode(EVSyncMode vsync_mode) override;
    };

}