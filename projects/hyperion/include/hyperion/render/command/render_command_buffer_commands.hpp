//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/render/render_buffer.hpp"
#include "hyperion/render/types/render_types_general.hpp"
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class RenderCommandBufferCommandType {
    ClearRenderTarget,
    SetRenderTarget,
    Blit,
    SetGlobalBuffer,
    RequestAsyncReadback,
  };

  struct RenderCommandBufferCommandClearRenderTarget {
    ClearFlags flags;
    Color color;
  };

  struct RenderCommandBufferCommandSetRenderTarget {
    RenderTargetId id;
  };

  struct RenderCommandBufferCommandBlit {
    RenderTargetId destination;
    RenderTargetId source;
  };

  struct RenderCommandBufferCommandSetGlobalBuffer {
    ShaderPropertyId id;
    RenderBuffer render_buffer;
  };

  struct AsyncRequestCallbackResult {
    RectInt region;

    Array<byte> data;
  };

  using AsyncRequestCallback = std::function<void(const AsyncRequestCallbackResult &)>;

  struct AsyncRequestResult {
    AsyncRequestCallback callback;
    AsyncRequestCallbackResult result;
  };

  struct RenderCommandBufferCommandRequestAsyncReadback {
    RenderTargetId render_target_id;
    uint32 attachment_index;
    RectInt region;
    AsyncRequestCallback callback;
  };

  struct RenderCommandBufferCommand {
    RenderCommandBufferCommandType type;

    std::variant<
      RenderCommandBufferCommandClearRenderTarget,
      RenderCommandBufferCommandSetRenderTarget,
      RenderCommandBufferCommandBlit,
      RenderCommandBufferCommandSetGlobalBuffer,
      RenderCommandBufferCommandRequestAsyncReadback
    > data;
  };

}