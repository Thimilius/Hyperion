//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    namespace Rendering {
        struct RenderFrameContextObjectUI;
    }

    namespace UI {
        class UIElement;
        struct UIViewComponent;
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
    public:
        static void Run(UI::UIViewComponent *ui_view, Delegate<RenderFrameContextObjectUI &()> ui_object_adder);
    private:
        static void RenderElement(UI::UIElement *element, Delegate<RenderFrameContextObjectUI &()> ui_object_adder);
    };

}