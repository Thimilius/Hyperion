#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderPipeline {
        Forward,
        Deferred,

        Custom
    };

    class IRenderPipeline {
    public:
        virtual ~IRenderPipeline() = default;
    public:
        virtual void Initialize() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;
    };

}