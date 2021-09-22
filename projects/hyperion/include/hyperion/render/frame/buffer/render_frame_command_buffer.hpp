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
    private:
        RenderFrameCommandBufferCommand &CreateCommand(RenderFrameCommandBufferCommandType type);
    private:
        Array<RenderFrameCommandBufferCommand> m_commands;
    };

}