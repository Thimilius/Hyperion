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

        inline RenderFrameContext &GetContext() { return m_context; }
        inline const RenderFrameContext &GetContext() const { return m_context; }
        inline const Array<RenderFrameCommand> &GetCommands() const { return m_commands; }
        inline const Array<AsyncRequest> &GetAsyncRequests() const { return m_async_requests; }

        CullingResults Cull(CullingParameters parameters);

        void SetCamera(uint64 camera_index);
        void ExecuteCommandBuffer(const RenderFrameCommandBuffer &command_buffer);
        void DrawMeshes(CullingResults &culling_results, DrawingParametes drawing_parameters);
        void DrawUI();
        void DrawEditorGizmos();
        void DrawEditorUI();

        AsyncRequest &AddAsyncRequests();
    private:
        RenderFrameCommand &CreateCommand(RenderFrameCommandType type);
    private:
        RenderFrameContext m_context;
        Array<RenderFrameCommand> m_commands;
        Array<AsyncRequest> m_async_requests;
    };

}