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
        SetRenderTarget,
        Blit,
        SetGlobalBuffer,
        RequestAsyncReadback,
    };

    struct RenderFrameCommandBufferCommandClearRenderTarget {
        ClearFlags flags;
        Color color;
    };
    
    struct RenderFrameCommandBufferCommandSetRenderTarget {
        RenderTargetId id;
    };

    struct RenderFrameCommandBufferCommandBlit {
        RenderTargetId destination;
        RenderTargetId source;
    };

    struct RenderFrameCommandBufferCommandSetGlobalBuffer {
        ShaderPropertyId id;
        RenderBuffer render_buffer;
    };

    struct AsyncRequestResult {
        RectInt region;

        Array<byte> data;
    };

    using AsyncRequestCallback = std::function<void(const AsyncRequestResult &)>;

    struct AsyncRequest {
        AsyncRequestResult result;
        AsyncRequestCallback callback;
    };

    struct RenderFrameCommandBufferCommandRequestAsyncReadback {
        RenderTargetId render_target_id;
        uint32 attachment_index;
        RectInt region;
        AsyncRequestCallback callback;
    };

    struct RenderFrameCommandBufferCommand {
        RenderFrameCommandBufferCommandType type;

        std::variant<
            RenderFrameCommandBufferCommandClearRenderTarget,
            RenderFrameCommandBufferCommandSetRenderTarget,
            RenderFrameCommandBufferCommandBlit,
            RenderFrameCommandBufferCommandSetGlobalBuffer,
            RenderFrameCommandBufferCommandRequestAsyncReadback
        > data;
    };

}