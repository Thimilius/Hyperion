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

    // TODO: This whole ui engine is kinda weird and should not exist in the way it does now.
    // We should probably move that into components and add back in the functionality that native components can be updated.
    class UiEngine final {
    private:
        UiEngine() = delete;
        ~UiEngine() = delete;
    private:
        static void Update();
        static void LateUpdate();

        static void UpdateCanvas(Canvas *canvas);
        static Vector<Widget *> RaycastWidgets(const Vector<Widget *> &widgets, Vec2 screen_position);
        static void SendEventMessage(Widget *widget, EventMessageType type);

        static void RegisterWidget(Widget *widget);
        static void UnregisterWidget(Widget *widget);
    private:
        inline static Vector<Widget *> s_widgets;
        inline static Widget *s_hovered_widget;

        inline static MeshBuilder s_mesh_builder;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Widget;
    };

}