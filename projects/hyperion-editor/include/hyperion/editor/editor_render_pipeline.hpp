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
        void Render(Rendering::RenderFrame *render_frame) override;
        void Shutdown() override;

        inline uint32 GetRenderTargetWidth() const override { return m_wrapped_pipeline->GetRenderTargetWidth(); }
        inline uint32 GetRenderTargetHeight() const override { return m_wrapped_pipeline->GetRenderTargetHeight(); }
        inline void SetRenderTargetSize(uint32 width, uint32 height) override { m_wrapped_pipeline->SetRenderTargetSize(width, height); }
        inline RenderTexture *GetTargetRenderTexture() const override { return m_wrapped_pipeline->GetTargetRenderTexture(); }

        inline void SetShouldBlitToScreen(bool8 should_blit_to_screen) override { m_wrapped_pipeline->SetShouldBlitToScreen(should_blit_to_screen); }
    private:
        Rendering::IRenderPipeline *m_wrapped_pipeline = nullptr;

        RenderTexture *m_object_ids_render_texture = nullptr;
    };

}