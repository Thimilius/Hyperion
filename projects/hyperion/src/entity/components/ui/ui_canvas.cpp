#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_canvas.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/ui/ui_graphic.hpp"

namespace Hyperion {

    void UICanvas::GetUIGraphics(Vector<UIGraphic *> &graphics) const {
        GetUIGraphicsInChildren(GetTransform(), graphics);
    }

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

    void UICanvas::GetUIGraphicsInChildren(Transform *transform, Vector<UIGraphic *> &graphics) const {
        UIGraphic *graphic = transform->GetEntity()->GetComponent<UIGraphic>();
        if (graphic != nullptr) {
            graphics.push_back(graphic);
        }
        
        for (u32 i = 0; i < transform->GetChildCount(); i++) {
            GetUIGraphicsInChildren(transform->GetChild(i), graphics);
        }
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UICanvas>(HYP_NAME_OF_TYPE(UICanvas));
}