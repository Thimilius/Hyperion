//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderFrame::Clear() {
        m_context.Clear();
        m_commands.Clear();
    }

    //--------------------------------------------------------------
    void RenderFrame::SetCamera(uint64 camera_index) {
        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::SetCamera);
        command.data.set_camera.camera_index = camera_index;
    }

    //--------------------------------------------------------------
    void RenderFrame::Clear(ClearFlags clear_flags, Color clear_color) {
        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::Clear);
        command.data.clear.flags = clear_flags;
        command.data.clear.color = clear_color;
    }

    //--------------------------------------------------------------
    void RenderFrame::DrawAll() {
        RenderFrameCommand &command = CreateCommand(RenderFrameCommandType::DrawAll);
    }

    //--------------------------------------------------------------
    RenderFrameCommand &RenderFrame::CreateCommand(RenderFrameCommandType type) {
        m_commands.Resize(m_commands.GetLength() + 1);
        RenderFrameCommand &command = m_commands.GetLast();
        command.type = type;
        return command;
    }

}