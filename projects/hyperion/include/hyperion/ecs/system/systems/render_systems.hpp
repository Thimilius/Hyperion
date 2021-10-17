#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    namespace Rendering {
        class RenderFrameContext;
    }

    namespace UI {
        class UIElement;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class MeshBoundsSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class EnvironmentSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class CameraSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class LightSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class SpriteRenderSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class MeshRenderSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class UIRenderSystem : public ISystem {
    public:
        void Run(World *world) override;
    private:
        void RenderElement(UI::UIElement *element, RenderFrameContext &render_frame_context);
    };

}