#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame_commands.hpp"
#include "hyperion/render/frame/buffer/render_frame_command_buffer.hpp"
#include "hyperion/render/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderFrame final : public INonCopyable {
    public:
        void Clear();

        RenderFrameContext &GetContext() { return m_context; }
        const RenderFrameContext &GetContext() const { return m_context; }
        const Array<RenderFrameCommand> &GetCommands() const { return m_commands; }

        CullingResults Cull(CullingParameters parameters);

        void SetCamera(uint64 camera_index);
        void ExecuteCommandBuffer(const RenderFrameCommandBuffer &command_buffer);
        void DrawMeshes(CullingResults &culling_results, DrawingParametes drawing_parameters);
        void DrawUI();
        void DrawEditorGizmos();
        void DrawEditorUI();
    private:
        RenderFrameCommand &CreateCommand(RenderFrameCommandType type);
    private:
        RenderFrameContext m_context;
        Array<RenderFrameCommand> m_commands;
    };

}