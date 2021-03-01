//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/canvas.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/rect_transform.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Canvas::UpdateScale() {
        if (Display::GetWidth() != m_cached_display_width || Display::GetHeight() != m_cached_display_height) {
            float32 display_width = static_cast<float32>(Display::GetWidth());
            float32 display_height = static_cast<float32>(Display::GetHeight());

            float32 log_base = 2;
            float32 log_width = Math::Log(display_width / m_reference_resolution.x) / Math::Log(log_base);
            float32 log_height = Math::Log(display_height / m_reference_resolution.y) / Math::Log(log_base);
            float32 log_weighted_average = Math::Lerp(log_width, log_height, 0.5f);
            float32 computed_scale = Math::Pow(log_base, log_weighted_average);

            m_cached_display_width = Display::GetWidth();
            m_cached_display_height = Display::GetHeight();
            m_scale = m_scale_factor * computed_scale;

            RectTransform *transform = GetEntity()->GetComponent<RectTransform>();
            HYP_ASSERT(transform);

            // Our actual size has to be determined based on the scale factor.
            float32 inverse_scale = 1.0f / m_scale;

            transform->SetSize(Vec2(inverse_scale * display_width, inverse_scale * display_height));
            transform->SetAnchoredPosition(Vec3(display_width / 2.0f, display_height / 2.0f, 0.0f));
            transform->SetLocalScale(Vec3(m_scale, m_scale, m_scale));
        }
    }

    //--------------------------------------------------------------
    void Canvas::OnCreate() {
        Component::OnCreate();

        UpdateScale();
    }

    //--------------------------------------------------------------
    Canvas *Canvas::Create() {
        return new Canvas();
    }

}