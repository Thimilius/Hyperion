#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,
    };

    struct RenderCommandClear {
        ClearMask clear_mask;
        Color color;
    };

    struct RenderCommand {
        RenderCommandType type;
        union {
            RenderCommandClear clear;
        };
    };

}