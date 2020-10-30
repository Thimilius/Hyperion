#include "hyppch.hpp"

#include "hyperion/entity/components/ui/canvas.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/ui/graphic.hpp"

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

        Vector<Graphic *> graphics;
        Vec2 mouse_position = Input::GetMousePosition();
        for (Graphic *graphic : m_graphics) {
            RectTransform *rect_transform = static_cast<RectTransform *>(graphic->GetTransform());
            if (RectTransformUtility::RectangleContainsScreenPoint(rect_transform, mouse_position)) {
                graphics.push_back(graphic);
            }
            graphic->SetColor(Color::White());
        }
        std::sort(graphics.begin(), graphics.end(), [](Graphic *first, Graphic *second) {
            return first->GetDepth() > second->GetDepth();
        });

        if (graphics.size() > 0) {
            Graphic *graphic = graphics[0];
            graphic->SetColor(Color::Red());
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

    void Canvas::AddGraphic(Graphic *graphic) {
        m_graphics.push_back(graphic);

        UpdateGraphicDepths();
    }

    void Canvas::RemoveGraphic(Graphic *graphic) {
        auto begin = m_graphics.begin();
        auto end = m_graphics.end();
        if (std::find(begin, end, graphic) != end) {
            m_graphics.erase(std::remove(begin, end, graphic));
        }

        UpdateGraphicDepths();
    }

    void Canvas::UpdateGraphicDepths() {
        Vector<Graphic *> graphics = GetEntity()->GetComponentsInChildren<Graphic>();
        s32 depth = 0;
        for (Graphic *graphic : graphics) {
            graphic->m_depth = depth++;
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