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
    RenderFrameCommandBufferCommand &RenderFrameCommandBuffer::CreateCommand(RenderFrameCommandBufferCommandType type) {
        m_commands.Resize(m_commands.GetLength() + 1);
        RenderFrameCommandBufferCommand &command = m_commands.GetLast();
        command.type = type;
        return command;
    }

}