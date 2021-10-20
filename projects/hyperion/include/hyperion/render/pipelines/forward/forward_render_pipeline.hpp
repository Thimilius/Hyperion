#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/render_frame.hpp"
#include "hyperion/render/pipelines/render_pipeline.hpp"
#include "hyperion/render/pipelines/forward/forward_render_lighting.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Material;
    class Mesh;
    class RenderTexture;
    class Shader;

    namespace Rendering {
        struct RenderFrameContextCamera;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize() override;
        void Render(RenderFrame *render_view) override;
        void Shutdown() override;

        void SetRenderTargetSize(uint32 width, uint32 height);
        inline RenderTexture *GetTargetRenderTexture() const { return m_target_render_texture; }
    private:
        uint32 m_render_target_width;
        uint32 m_render_target_height;
        RenderTexture *m_target_render_texture;
    };

}