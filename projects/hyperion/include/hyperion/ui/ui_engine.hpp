#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/app/events/event.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Widget;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class UiEngine final {
    private:
        UiEngine() = delete;
        ~UiEngine() = delete;
    private:
        static void Update();
        static void LateUpdate();

        static void RegisterWidget(Widget *widget);
        static void UnregisterWidget(Widget *widget);
    private:
        inline static Vector<Widget *> s_widgets;

        inline static MeshBuilder s_mesh_builder;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Widget;
    };

}