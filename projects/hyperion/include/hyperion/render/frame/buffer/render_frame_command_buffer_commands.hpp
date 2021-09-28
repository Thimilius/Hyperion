#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/render/render_buffer.hpp"
#include "hyperion/render/types/render_types_general.hpp"
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderFrameCommandBufferCommandType {
        ClearRenderTarget,
        SetGlobalBuffer,
    };

    struct RenderFrameCommandBufferCommandClearRenderTarget {
        ClearFlags flags;
        Color color;
    };

    struct RenderFrameCommandBufferCommandSetGlobalBuffer {
        ShaderPropertyId id;
        RenderBuffer render_buffer;
    };

    struct RenderFrameCommandBufferCommand {
        RenderFrameCommandBufferCommandType type;

        std::variant<
            RenderFrameCommandBufferCommandClearRenderTarget,
            RenderFrameCommandBufferCommandSetGlobalBuffer
        > data;
    };

}