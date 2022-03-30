//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"
#include "hyperion/render/assets/render_assets_context.hpp"
#include "hyperion/render/command/render_command_buffer.hpp"
#include "hyperion/render/frame/render_frame_commands.hpp"
#include "hyperion/render/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {


  class RenderFrame final : public INonCopyable {
  public:
    void Clear();

    inline RenderFrameContext &GetContext() { return m_context; }
    inline const RenderFrameContext &GetContext() const { return m_context; }
    inline RenderAssetContext &GetAssetContext() { return m_asset_context; }
    inline const RenderAssetContext &GetAssetContext() const { return m_asset_context; }
    inline const Array<RenderFrameCommand> &GetCommands() const { return m_commands; }

    CullingResults Cull(CullingParameters parameters);

    void SetCamera(uint64 camera_index);
    void ExecuteCommandBuffer(const RenderCommandBuffer &command_buffer);
    void DrawMeshes(CullingResults &culling_results, DrawingParameters drawing_parameters);
    void DrawUI();
    void DrawObjectIds(RenderTargetId render_target_id);
    void DrawGizmos();
    void DrawEditorUI();

    inline const Array<AsyncRequestResult> &GetAsyncRequestResults() const { return m_async_request_results; }
    AsyncRequestResult &AddAsyncRequestResult();
  private:
    RenderFrameCommand &CreateCommand(RenderFrameCommandType type);
  private:
    RenderFrameContext m_context;
    RenderAssetContext m_asset_context;
    Array<RenderFrameCommand> m_commands;
    Array<AsyncRequestResult> m_async_request_results;
  };

}