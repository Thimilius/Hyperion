//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/command/render_command_buffer_commands.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderCommandBuffer {
  public:
    void Clear();

    const Array<RenderCommandBufferCommand> &GetCommands() const { return m_commands; }

    void ClearRenderTarget(ClearFlags clear_flags, Color clear_color);
    void SetRenderTarget(RenderTargetId id);
    void Blit(RenderTargetId destination, RenderTargetId source);

    void SetGlobalBuffer(ShaderPropertyId id, RenderBuffer &&render_buffer);

    void RequestAsyncReadback(RenderTargetId id, uint32 attachment_index, RectInt region, AsyncRequestCallback callback);
  private:
    RenderCommandBufferCommand &CreateCommand(RenderCommandBufferCommandType type);
  private:
    Array<RenderCommandBufferCommand> m_commands;
  };

}
