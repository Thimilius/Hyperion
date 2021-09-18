#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderFrame;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class IRenderDriver {
    public:
        virtual ~IRenderDriver() = default;
    public:
        virtual void Initialize() = 0;
        virtual void Render(RenderFrame *render_frame) = 0;
        virtual void Shutdown() = 0;
    };

}