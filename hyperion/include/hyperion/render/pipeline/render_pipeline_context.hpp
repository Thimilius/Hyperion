//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/non_copyable.hpp"
#include "hyperion/render/command/render_command_buffer.hpp"
#include "hyperion/render/pipeline/render_pipeline_context_commands.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class RenderFrame;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderPipelineContext final : public INonCopyable {
  public:
    void Clear();

    inline RenderFrame *GetRenderFrame() const { return m_render_frame; }
    
    inline const Array<RenderPipelineCommand> &GetCommands() const { return m_commands; }
  
    CullingResults Cull(CullingParameters parameters);

    void SetCamera(uint64 camera_index);
    void ExecuteCommandBuffer(const RenderCommandBuffer &command_buffer);
    void DrawMeshes(CullingResults &culling_results, DrawingParameters drawing_parameters);
    void DrawShadows(ShadowParameters shadow_parameters);
    void DrawUI();
    void DrawObjectIds(RenderTargetId render_target_id);
    void DrawGizmos();
    void DrawEditorUI();

    inline const Array<AsyncRequestResult> &GetAsyncRequestResults() const { return m_async_request_results; }
    AsyncRequestResult &AddAsyncRequestResult();
  private:
    RenderPipelineCommand &CreateCommand(RenderPipelineCommandType type);
  private:
    RenderFrame *m_render_frame = nullptr;
    
    Array<RenderPipelineCommand> m_commands;
    Array<AsyncRequestResult> m_async_request_results;
  private:
    friend class Hyperion::Rendering::RenderFrame;
  };
  
}
