//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/buffer/render_frame_command_buffer_commands.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderFrameCommandBuffer {
  public:
    void Clear();

    const Array<RenderFrameCommandBufferCommand> &GetCommands() const { return m_commands; }

    void ClearRenderTarget(ClearFlags clear_flags, Color clear_color);
    void SetRenderTarget(RenderTargetId id);
    void Blit(RenderTargetId destination, RenderTargetId source);

    void SetGlobalBuffer(ShaderPropertyId id, RenderBuffer &&render_buffer);

    void RequestAsyncReadback(RenderTargetId id, uint32 attachment_index, RectInt region, AsyncRequestCallback callback);
  private:
    RenderFrameCommandBufferCommand &CreateCommand(RenderFrameCommandBufferCommandType type);
  private:
    Array<RenderFrameCommandBufferCommand> m_commands;
  };

}