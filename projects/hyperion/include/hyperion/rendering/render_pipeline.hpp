#pragma once

namespace Hyperion::Rendering {

    class RenderPipeline {
    public:
        virtual ~RenderPipeline() = default;

        virtual void Render() = 0;
    };

}