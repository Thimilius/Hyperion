#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/driver/opengl/opengl_render_driver_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class WindowsOpenGLRenderDriverContext : public OpenGLRenderDriverContext {
    public:
        WindowsOpenGLRenderDriverContext(HDC device_context, HDC helper_device_context);
    public:
        void Initialize(const RenderDriverContextDescriptor &descriptor) override;
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