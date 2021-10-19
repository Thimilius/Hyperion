#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UIRebuildSystem : public ISystem {
    public:
        void Run(World *world) override;
    private:
        static void Rebuild(UIElement *element, float32 ui_scale, MeshBuilder &mesh_builder);
    };

    class UIEventSystem : public ISystem {
    public:
        void Run(World *world) override;
    private:
        static void RaycastElements(UIElement *element, Vector2 screen_point, Array<UIElement *> &raycasted);
    };

}