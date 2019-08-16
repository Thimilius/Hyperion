#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion::Rendering {

    struct SContextProperties {
        TString vendor;
        TString renderer;
        TString version;

        s32 max_samples;
    };
    
    class CGraphicsContext {
    public:
        virtual SContextProperties GetProperties() = 0;
    protected:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;

        friend class CWindowsWindow;
    };

}