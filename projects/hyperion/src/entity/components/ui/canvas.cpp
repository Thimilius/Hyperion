#include "hyppch.hpp"

#include "hyperion/entity/components/ui/canvas.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

namespace Hyperion::UI {

    void Canvas::OnCreate() {
        Component::OnCreate();

        RegisterForUpdate();
        GetWorld()->AddUICanvas(this);
    }

    void Canvas::OnUpdate(f32 delta_time) {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        if (display_width != m_cached_display_width || display_height != m_cached_display_height) {
            UpdateScale();
        }

        Vector<Widget *> widgets;
        Vec2 mouse_position = Input::GetMousePosition();
        for (Widget *widget : m_widgets) {
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

    void Canvas::OnDestroy() {
        GetWorld()->RemoveUICanvas(this);
        UnregisterForUpdate();

        Component::OnDestroy();
    }

    void Canvas::UpdateScale() {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        f32 log_base = 2;
        f32 log_width = Math::Log(display_width / m_reference_resolution.x) / Math::Log(log_base);
        f32 log_height = Math::Log(display_height / m_reference_resolution.y) / Math::Log(log_base);
        f32 log_weighted_average = Math::Lerp(log_width, log_height, 0.5f);
        f32 scale_factor = Math::Pow(log_base, log_weighted_average);

        m_cached_display_width = display_width;
        m_cached_display_height = display_height;
        m_full_scale = m_scale * scale_factor;

        RectTransform *transform = GetEntity()->GetComponent<RectTransform>();
        transform->SetSize(Vec2(display_width, display_height));

        GetTransform()->SetLocalScale(Vec3(m_full_scale, m_full_scale, m_full_scale));
    }

    void Canvas::AddWidget(Widget *graphic) {
        m_widgets.push_back(graphic);

        UpdateWidgetDepths();
    }

    void Canvas::RemoveWidget(Widget *graphic) {
        auto begin = m_widgets.begin();
        auto end = m_widgets.end();
        if (std::find(begin, end, graphic) != end) {
            m_widgets.erase(std::remove(begin, end, graphic));
        }

        UpdateWidgetDepths();
    }

    void Canvas::UpdateWidgetDepths() {
        Vector<Widget *> widgets = GetEntity()->GetComponentsInChildren<Widget>();
        s32 depth = 0;
        for (Widget *widget : widgets) {
            widget->m_depth = depth++;
        }
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;
    using namespace Hyperion::UI;

    registration::class_<Canvas>(HYP_NAME_OF_TYPE(Canvas));
}