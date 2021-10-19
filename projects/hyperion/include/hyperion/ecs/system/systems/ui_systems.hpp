#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UILayoutSystem : public ISystem {
    public:
        void Run(World *world) override;
    private:
        static void Rebuild(UIElement *element, MeshBuilder &mesh_builder);
    };

    class UIEventSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}