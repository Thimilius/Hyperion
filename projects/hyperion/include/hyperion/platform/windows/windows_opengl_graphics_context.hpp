#pragma once

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"

#include <Windows.h>

namespace Hyperion::Rendering {

    class WindowsOpenGLGraphicsContext : public OpenGLGraphicsContext {
    public:
        WindowsOpenGLGraphicsContext(HDC device_context, HDC helper_device_context);
        ~WindowsOpenGLGraphicsContext() override;
    private:
        void Init() override;
        void Present() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;

        static void LoadOpenGLExtensions(HDC helper_device_context);
    private:
        HDC m_device_context;
        HGLRC m_opengl_context;
    };

}