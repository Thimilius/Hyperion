#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,
    };

    struct RenderCommandExit { };

    struct RenderCommandClear {
        ClearMask clear_mask;
        Color color;
    };

}