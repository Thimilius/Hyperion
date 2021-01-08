#pragma once

namespace Hyperion::Rendering {

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void Init() { }
        virtual void Render() { }
        virtual void Shutdown() { }
    };

}