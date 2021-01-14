#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_mask, Color color) override;
    private:
        static u32 GetGLClearMask(ClearFlags clear_mask);
    };

}