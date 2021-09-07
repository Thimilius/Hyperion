#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    namespace Graphics {
        class GraphicsContext;
    }
    namespace Rendering {
        class RenderFrame;
    }
}

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
        virtual void Initialize(Graphics::GraphicsContext *graphics_context) = 0;
        virtual void Render(RenderFrame *render_frame) = 0;
        virtual void Shutdown() = 0;
    };

}