//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/pipeline/render_pipeline.hpp"
#include "hyperion/render/pipeline/forward/forward_render_lighting.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Material;
  class Mesh;
  class RenderTexture;
  class Shader;

  namespace Rendering {
    struct RenderObjectContextCamera;
  }}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class ForwardRenderPipeline final : public IRenderPipeline {
  public:
    void Initialize() override;
    void Render(RenderPipelineContext &context, const Array<const RenderObjectContextCamera *> &cameras) override;
    void Shutdown() override;
    
    void SetupRendering(RenderPipelineContext &context) override;
    void RenderCamera(RenderPipelineContext &context, const RenderObjectContextCamera *camera, RenderTexture *target_texture) override;

    inline uint32 GetRenderTargetWidth() const override { return m_render_target_width; }
    inline uint32 GetRenderTargetHeight() const override { return m_render_target_height; }
    void SetRenderTargetSize(uint32 width, uint32 height) override;
    inline RenderTexture *GetTargetRenderTexture() const override { return m_target_render_texture; }

    inline void SetShouldDoSetup(bool8 should_do_setup) override { m_should_do_setup = should_do_setup; }
    inline void SetShouldBlitToScreen(bool8 should_blit_to_screen) override { m_should_blit_to_screen = should_blit_to_screen; }
    inline void SetShouldResizeToScreen(bool8 should_resize_to_screen) override { m_should_resize_to_screen = should_resize_to_screen; }
    inline void SetShouldDrawGizmos(bool8 should_draw_gizmos) override { m_should_draw_gizmos = should_draw_gizmos; }
  private:
    void DrawShadows(RenderPipelineContext &context);
    void DrawMeshes(RenderPipelineContext &context, const RenderObjectContextCamera *camera, CullingResults &culling_results, RenderTexture *target_texture);
  private:
    ForwardRenderLighting m_lighting;
    
    uint32 m_render_target_width = 0;
    uint32 m_render_target_height = 0;
    RenderTexture *m_target_render_texture = nullptr;
    RenderTexture *m_shadow_render_texture = nullptr;

    bool8 m_should_do_setup = true;
    bool8 m_should_blit_to_screen = true;
    bool8 m_should_resize_to_screen = true;
    bool8 m_should_draw_gizmos = true;
  private:
    inline static constexpr uint32 SHADOW_MAP_SIZE = 2048;
  };

}