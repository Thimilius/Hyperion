#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Mesh;

    namespace Rendering {
        struct RenderFrameCamera;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize(Graphics::GraphicsContext *graphics_context) override;
        void Render(RenderFrame *render_view) override;
        void Shutdown() override;
    private:
        void RenderCamera(const RenderFrameCamera &render_frame_camera, RenderFrame *render_frame);

        void HandleAssets(RenderFrame *render_frame);
        void LoadMesh(Mesh *mesh);
    private:
        Graphics::GraphicsDevice *m_device;
        Graphics::GraphicsDeviceContext *m_device_context;
        Graphics::GraphicsSwapChain *m_swap_chain;
    };

}