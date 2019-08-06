#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion::Rendering {

    class CGraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;
    };

}