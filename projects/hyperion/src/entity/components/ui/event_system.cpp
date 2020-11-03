#include "hyppch.hpp"

#include "hyperion/entity/components/ui/event_system.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

namespace Hyperion::UI {

    void EventSystem::OnCreate() {
        if (s_current) {
            // TODO: Better error handling
            HYP_ERROR("Multiple event systems are not supported!");
            return;
        }

        s_current = this;
        RegisterForUpdate();
    }

    void EventSystem::OnUpdate(f32 delta_time) {
        const Vector<Canvas *> &canvases = GetWorld()->GetCanvases();
        if (canvases.size() == 0) {
            return;
        }
        // FIXME: We only support one canvas for now
        Canvas *canvas = canvases[0];

        Vector<Widget *> widgets;
        Vec2 mouse_position = Input::GetMousePosition();
        for (Widget *widget : WidgetRegistry::GetWidgetsForCanvas(canvas)) {
            RectTransform *rect_transform = static_cast<RectTransform *>(widget->GetTransform());
            if (RectTransformUtility::RectangleContainsScreenPoint(rect_transform, mouse_position)) {
                widgets.push_back(widget);
            }
            widget->SetColor(Color::White());
        }
        std::sort(widgets.begin(), widgets.end(), [](Widget *first, Widget *second) {
            return first->GetDepth() > second->GetDepth();
        });

        if (widgets.size() > 0) {
            Widget *widget = widgets[0];
            widget->SetColor(Color::Red());
        }
    }

    void EventSystem::OnDestroy() {
        s_current = nullptr;
        UnregisterForUpdate();
    }

}