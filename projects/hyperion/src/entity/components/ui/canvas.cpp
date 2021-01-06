#include "hyppch.hpp"

#include "hyperion/entity/components/ui/canvas.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

namespace Hyperion::UI {

    void Canvas::OnCreate() {
        Component::OnCreate();

        RegisterForUpdate();
        GetWorld()->AddCanvas(this);
    }

    void Canvas::OnUpdate(f32 delta_time) {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        if (display_width != m_cached_display_width || display_height != m_cached_display_height) {
            UpdateScale();
        }
    }

    void Canvas::OnDestroy() {
        GetWorld()->RemoveCanvas(this);
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

    void WidgetRegistry::RegisterWidget(Canvas *canvas, Widget *widget) {
        s_widgets[canvas].push_back(widget);
    }

    void WidgetRegistry::UnregisterWidget(Canvas *canvas, Widget *widget) {
        auto &widgets = s_widgets[canvas];
        auto begin = widgets.begin();
        auto end = widgets.end();
        if (std::find(begin, end, widget) != end) {
            widgets.erase(std::remove(begin, end, widget));
        }
    }

    const Vector<Widget *> &WidgetRegistry::GetWidgetsForCanvas(Canvas *canvas) {
        auto it = s_widgets.find(canvas);
        if (it != s_widgets.end()) {
            return it->second;
        } else {
            return s_empty_widgets;
        }
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<UI::Canvas>("Canvas");
}
HYP_REFLECT_REGISTER_END