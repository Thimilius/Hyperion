#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_canvas.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/ui/ui_graphic.hpp"

namespace Hyperion {

    void UICanvas::OnCreate() {
        Component::OnCreate();

        RegisterForUpdate();
        GetWorld()->AddUICanvas(this);
    }

    void UICanvas::OnUpdate(f32 delta_time) {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        if (display_width != m_cached_display_width || display_height != m_cached_display_height) {
            UpdateScale();
        }

        
    }

    void UICanvas::OnDestroy() {
        GetWorld()->RemoveUICanvas(this);
        UnregisterForUpdate();

        Component::OnDestroy();
    }

    void UICanvas::UpdateScale() {
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

        UITransform *transform = GetEntity()->GetComponent<UITransform>();
        transform->SetSize(Vec2(display_width, display_height));

        GetTransform()->SetLocalScale(Vec3(m_full_scale, m_full_scale, m_full_scale));
    }

    void UICanvas::AddUIGraphic(UIGraphic *ui_graphic) {
        m_ui_graphics.push_back(ui_graphic);

        UpdateUIGraphicDepths();
    }

    void UICanvas::RemoveUIGraphic(UIGraphic *ui_graphic) {
        auto begin = m_ui_graphics.begin();
        auto end = m_ui_graphics.end();
        if (std::find(begin, end, ui_graphic) != end) {
            m_ui_graphics.erase(std::remove(begin, end, ui_graphic));
        }

        UpdateUIGraphicDepths();
    }

    void UICanvas::UpdateUIGraphicDepths() {
        Vector<UIGraphic *> graphics = GetEntity()->GetComponentsInChildren<UIGraphic>();
        s32 depth = 0;
        for (UIGraphic *graphic : graphics) {
            graphic->m_depth = depth++;
        }
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UICanvas>(HYP_NAME_OF_TYPE(UICanvas));
}