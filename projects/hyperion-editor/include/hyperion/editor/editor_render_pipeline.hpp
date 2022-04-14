//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/assets/texture.hpp>
#include <hyperion/render/pipelines/render_pipeline.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorRenderPipeline : public Rendering::IRenderPipeline {
  public:
    EditorRenderPipeline();
  public:
    void Initialize() override;
    void Render(Rendering::RenderFrame *render_frame, const Array<const Rendering::RenderFrameContextCamera *> cameras) override;
    void RenderCamera(Rendering::RenderFrame *render_frame, const Rendering::RenderFrameContextCamera *camera) override;
    void Shutdown() override;

    inline uint32 GetRenderTargetWidth() const override { return m_wrapped_pipeline->GetRenderTargetWidth(); }
    inline uint32 GetRenderTargetHeight() const override { return m_wrapped_pipeline->GetRenderTargetHeight(); }
    inline void SetRenderTargetSize(uint32 width, uint32 height) override { m_wrapped_pipeline->SetRenderTargetSize(width, height); }
    inline RenderTexture *GetTargetRenderTexture() const override { return m_wrapped_pipeline->GetTargetRenderTexture(); }
    inline RenderTexture *GetEditorTargetRenderTexture() const { return m_editor_render_texture; }

    inline void SetShouldBlitToScreen(bool8 should_blit_to_screen) override { m_wrapped_pipeline->SetShouldBlitToScreen(should_blit_to_screen); }
    inline void SetShouldResizeToScreen(bool8 should_resize_to_screen) override { m_wrapped_pipeline->SetShouldResizeToScreen(should_resize_to_screen); }
  private:
    void RenderEditor(Rendering::RenderFrame *render_frame);
    void UpdateSize();
  private:
    Rendering::IRenderPipeline *m_wrapped_pipeline = nullptr;

    RenderTexture *m_editor_render_texture = nullptr;
    RenderTexture *m_object_ids_render_texture = nullptr;
  };

}
