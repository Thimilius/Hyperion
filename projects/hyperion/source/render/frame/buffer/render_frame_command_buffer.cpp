//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/buffer/render_frame_command_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::Clear() {
        m_commands.Clear();
    }

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::ClearRenderTarget(ClearFlags clear_flags, Color clear_color) {
        RenderFrameCommandBufferCommandClearRenderTarget clear_render_target;
        clear_render_target.flags = clear_flags;
        clear_render_target.color = clear_color;

        RenderFrameCommandBufferCommand &command = CreateCommand(RenderFrameCommandBufferCommandType::ClearRenderTarget);
        command.data = clear_render_target;
    }

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::SetRenderTarget(RenderTargetId id) {
        RenderFrameCommandBufferCommandSetRenderTarget set_render_target;
        set_render_target.id = id;

        RenderFrameCommandBufferCommand &command = CreateCommand(RenderFrameCommandBufferCommandType::SetRenderTarget);
        command.data = set_render_target;
    }

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::Blit(RenderTargetId destination, RenderTargetId source) {
        RenderFrameCommandBufferCommandBlit blit;
        blit.destination = destination;
        blit.source = source;

        RenderFrameCommandBufferCommand &command = CreateCommand(RenderFrameCommandBufferCommandType::Blit);
        command.data = blit;
    }

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::SetGlobalBuffer(ShaderPropertyId id, RenderBuffer &&render_buffer) {
        RenderFrameCommandBufferCommandSetGlobalBuffer set_global_buffer;
        set_global_buffer.id = id;
        set_global_buffer.render_buffer = render_buffer;

        RenderFrameCommandBufferCommand &command = CreateCommand(RenderFrameCommandBufferCommandType::SetGlobalBuffer);
        command.data = set_global_buffer;
    }

    //--------------------------------------------------------------
    void RenderFrameCommandBuffer::RequestAsyncReadback(RenderTargetId id, uint32 attachment_index, RectInt region, AsyncRequestCallback callback) {
        RenderFrameCommandBufferCommandRequestAsyncReadback request_async_readback;
        request_async_readback.render_target_id = id;
        request_async_readback.attachment_index = attachment_index;
        request_async_readback.region = region;
        request_async_readback.callback = callback;

        RenderFrameCommandBufferCommand &command = CreateCommand(RenderFrameCommandBufferCommandType::RequestAsyncReadback);
        command.data = request_async_readback;
    }

    //--------------------------------------------------------------
    RenderFrameCommandBufferCommand &RenderFrameCommandBuffer::CreateCommand(RenderFrameCommandBufferCommandType type) {
        m_commands.Resize(m_commands.GetLength() + 1);
        RenderFrameCommandBufferCommand &command = m_commands.GetLast();
        command.type = type;
        return command;
    }

}