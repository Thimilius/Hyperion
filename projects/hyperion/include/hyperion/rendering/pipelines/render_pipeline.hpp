#pragma once

namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deffered,

        Custom
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;

        virtual void Init() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;
    };

}