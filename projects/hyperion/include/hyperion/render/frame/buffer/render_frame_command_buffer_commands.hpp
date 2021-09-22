#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/render/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderFrameCommandBufferCommandType {
        ClearRenderTarget,
    };

    struct RenderFrameCommandBufferCommandClearRenderTarget {
        ClearFlags flags;
        Color color;
    };

    struct RenderFrameCommandBufferCommand {
        RenderFrameCommandBufferCommandType type;

        std::variant<
            RenderFrameCommandBufferCommandClearRenderTarget
        > data;
    };

}