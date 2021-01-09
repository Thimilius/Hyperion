#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    class RenderOperation {
    public:
        inline static void Clear(ClearMask clear_mask, Color color) {
            s_render_driver->Clear(clear_mask, color);
        }
    private:
        inline static IRenderDriver *s_render_driver;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}