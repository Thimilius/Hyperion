#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion::Rendering {

    struct ContextProperties {
        String vendor;
        String renderer;
        String version;
    };
    
    class GraphicsContext {
    protected:
        ContextProperties m_properties;
    public:
        inline const ContextProperties &GetProperties() const { return m_properties; }
    protected:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;

        friend class WindowsWindow;
    };

}