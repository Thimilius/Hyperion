//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/command/render_command_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderCommandBuffer::Clear() {
    m_commands.Clear();
  }

  //--------------------------------------------------------------
  void RenderCommandBuffer::ClearRenderTarget(ClearFlags clear_flags, Color clear_color) {
    RenderCommandBufferCommandClearRenderTarget clear_render_target;
    clear_render_target.flags = clear_flags;
    clear_render_target.color = clear_color;

    RenderCommandBufferCommand &command = CreateCommand(RenderCommandBufferCommandType::ClearRenderTarget);
    command.data = clear_render_target;
  }

  //--------------------------------------------------------------
  void RenderCommandBuffer::SetRenderTarget(RenderTargetId id) {
    RenderCommandBufferCommandSetRenderTarget set_render_target;
    set_render_target.id = id;

    RenderCommandBufferCommand &command = CreateCommand(RenderCommandBufferCommandType::SetRenderTarget);
    command.data = set_render_target;
  }

  //--------------------------------------------------------------
  void RenderCommandBuffer::Blit(RenderTargetId destination, RenderTargetId source) {
    RenderCommandBufferCommandBlit blit;
    blit.destination = destination;
    blit.source = source;

    RenderCommandBufferCommand &command = CreateCommand(RenderCommandBufferCommandType::Blit);
    command.data = blit;
  }

  //--------------------------------------------------------------
  void RenderCommandBuffer::SetGlobalBuffer(ShaderPropertyId id, RenderBuffer &&render_buffer) {
    RenderCommandBufferCommandSetGlobalBuffer set_global_buffer;
    set_global_buffer.id = id;
    set_global_buffer.render_buffer = render_buffer;

    RenderCommandBufferCommand &command = CreateCommand(RenderCommandBufferCommandType::SetGlobalBuffer);
    command.data = set_global_buffer;
  }

  //--------------------------------------------------------------
  void RenderCommandBuffer::RequestAsyncReadback(RenderTargetId id, uint32 attachment_index, RectInt region, AsyncRequestCallback callback) {
    RenderCommandBufferCommandRequestAsyncReadback request_async_readback;
    request_async_readback.render_target_id = id;
    request_async_readback.attachment_index = attachment_index;
    request_async_readback.region = region;
    request_async_readback.callback = callback;

    RenderCommandBufferCommand &command = CreateCommand(RenderCommandBufferCommandType::RequestAsyncReadback);
    command.data = request_async_readback;
  }

  //--------------------------------------------------------------
  RenderCommandBufferCommand &RenderCommandBuffer::CreateCommand(RenderCommandBufferCommandType type) {
    m_commands.Resize(m_commands.GetLength() + 1);
    RenderCommandBufferCommand &command = m_commands.GetLast();
    command.type = type;
    return command;
  }

}
