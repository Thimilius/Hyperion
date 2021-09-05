#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize(Graphics::GraphicsContext *graphics_context) override;
        void Render(RenderView *render_view) override;
        void Shutdown() override;
    private:
        Graphics::GraphicsDevice *m_device;
        Graphics::GraphicsDeviceContext *m_device_context;
        Graphics::GraphicsSwapChain *m_swap_chain;
    };

}