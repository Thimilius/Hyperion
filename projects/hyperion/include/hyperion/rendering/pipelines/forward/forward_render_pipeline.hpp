#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize() override;
        void Render() override;
        void Shutdown() override;
    };

}