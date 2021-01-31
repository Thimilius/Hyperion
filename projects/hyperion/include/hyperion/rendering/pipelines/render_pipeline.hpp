#pragma once

namespace Hyperion::Rendering {
    class IRenderDriver;
}

namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class RenderPipelineContext {

    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void Initialize(IRenderDriver *render_driver) = 0;
        virtual void Render(IRenderDriver *render_driver, const RenderPipelineContext &context) = 0;
        virtual void Shutdown(IRenderDriver *render_driver) = 0;
    };

}