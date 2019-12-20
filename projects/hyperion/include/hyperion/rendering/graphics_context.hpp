#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion::Rendering {

    struct SContextProperties {
        TString vendor;
        TString renderer;
        TString version;
    };
    
    class CGraphicsContext {
    protected:
        SContextProperties m_properties;
    public:
        inline const SContextProperties &GetProperties() const { return m_properties; }
    protected:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;

        friend class CWindowsWindow;
    };

}