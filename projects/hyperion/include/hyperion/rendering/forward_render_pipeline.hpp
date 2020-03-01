#pragma once

#include "hyperion/rendering/render_pipeline.hpp"

namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public RenderPipeline {
    public:
        void Render(const CameraData &camera) override;
    };

}