#pragma once

namespace Hyperion::Rendering {

    class RenderPipeline {
    public:
        virtual ~RenderPipeline() = default;

        virtual void Init() { }
        virtual void Render() { }
    };

}