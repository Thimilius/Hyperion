#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class ForwardRenderPipeline : public IRenderPipeline {
    public:
        void Initialize(IRenderDriver *render_driver) override;
        void Render(IRenderDriver *render_driver) override;
        void Shutdown(IRenderDriver *render_driver) override;
    };

}