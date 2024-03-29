//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/pipeline/render_pipeline_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class RenderTexture;

  namespace Rendering {
    class RenderFrame;
    struct RenderObjectContextCamera;
  }}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class RenderPipeline {
    Forward,
    Deferred,

    Custom
  };

  class IRenderPipeline {
  public:
    virtual ~IRenderPipeline() = default;
  public:
    virtual void Initialize() = 0;
    virtual void Render(RenderPipelineContext &context, const Array<const RenderObjectContextCamera *> &cameras) = 0;
    virtual void Shutdown() = 0;
    
    virtual void SetupRendering(RenderPipelineContext &context) = 0;
    virtual void RenderCamera(RenderPipelineContext &context, const RenderObjectContextCamera *, RenderTexture *target_texture) = 0;

    virtual uint32 GetRenderTargetWidth() const = 0;
    virtual uint32 GetRenderTargetHeight() const = 0;
    virtual void SetRenderTargetSize(uint32 width, uint32 height) = 0;
    virtual RenderTexture *GetTargetRenderTexture() const = 0;

    virtual void SetShouldDoSetup(bool8 should_clear_default_target) = 0;
    virtual void SetShouldBlitToScreen(bool8 should_blit_to_screen) = 0;
    virtual void SetShouldResizeToScreen(bool8 should_resize_to_screen) = 0;
    virtual void SetShouldDrawGizmos(bool8 should_draw_gizmos) = 0;
  };

}
