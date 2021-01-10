#pragma once

#include "hyperion/rendering/pipelines/render_pipeline.hpp"

namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Init() override { }
        void Render() override;
        void Shutdown() override { }
    };

}