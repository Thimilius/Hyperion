#pragma once

#include "hyperion/rendering/pipelines/render_context.hpp"

namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void Init(IRenderDriver *render_driver) = 0;
        virtual void Render(IRenderDriver *render_driver, const RenderContext &context) = 0;
        virtual void Shutdown(IRenderDriver *render_driver) = 0;
    };

}