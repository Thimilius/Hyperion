#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class WindowsOpenGLGraphicsContext : public OpenGLGraphicsContext {
    public:
        WindowsOpenGLGraphicsContext(HDC device_context, HDC helper_device_context);
    public:
        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;
        
        void SwapBuffers() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;
    private:
        void LoadOpenGLExtensions(HDC helper_device_context);
    private:
        HDC m_device_context;
        HGLRC m_opengl_context;
    };

}