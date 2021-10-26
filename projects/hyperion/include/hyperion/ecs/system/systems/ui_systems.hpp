#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
    struct UIViewComponent;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UIRebuildSystem : public ISystem {
    public:
        void Run(World *world) override;
    public:
        static void Run(UIViewComponent *ui_view);
    private:
        static void Rebuild(UIElement *element, float32 ui_scale, MeshBuilder &mesh_builder);
    };

    class UIEventSystem : public ISystem {
    public:
        void Run(World *world) override;
    public:
        static void Run(UIViewComponent *ui_view);
    private:
        static void RaycastElements(UIElement *element, Vector2Int screen_point, Array<UIElement *> &raycasted);
        static void SendEvent(UIElement *element, UIEventType type);
    };

}