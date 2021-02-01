#pragma once

#include "hyperion/rendering/pipelines/render_pipeline.hpp"

namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize(IRenderDriver *render_driver) override;
        void Render(IRenderDriver *render_driver, const RenderPipelineContext &context) override;
        void Shutdown(IRenderDriver *render_driver) override;
    };

}