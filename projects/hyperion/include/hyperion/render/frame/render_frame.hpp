#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame_commands.hpp"
#include "hyperion/render/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Clear();

        RenderFrameContext &GetContext() { return m_context; }
        const RenderFrameContext &GetContext() const { return m_context; }
        const Array<RenderFrameCommand> &GetCommands() const { return m_commands; }

        void SetCamera(uint64 camera_index);
        void Clear(ClearFlags clear_flags, Color clear_color);
        void DrawMeshes();
        void DrawGizmos();
    private:
        RenderFrameCommand &CreateCommand(RenderFrameCommandType type);
    private:
        RenderFrameContext m_context;
        Array<RenderFrameCommand> m_commands;
    };

}