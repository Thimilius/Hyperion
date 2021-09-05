#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    namespace Graphics {
        class GraphicsContext;
    }
    namespace Rendering {
        class RenderView;
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
        virtual void Render(RenderView *render_view) = 0;
        virtual void Shutdown() = 0;
    };

}