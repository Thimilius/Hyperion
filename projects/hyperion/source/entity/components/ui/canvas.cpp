//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/canvas.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Canvas::OnCreate() {
        Component::OnCreate();

        UpdateScale();

        EnableUpdate();
    }

    //--------------------------------------------------------------
    void Canvas::OnUpdate(float32 delta_time) {
        Vector<Widget *> hovered_widgets = RaycastWidgets(Input::GetMousePosition());

        if (hovered_widgets.size() > 0) {
            Widget *hovered_widget = hovered_widgets[0];

            if (m_hovered_widget != hovered_widget) {
                if (m_hovered_widget != nullptr) {
                    SendEventMessage(m_hovered_widget, EventMessageType::PointerExit);
                }
                m_hovered_widget = hovered_widget;

                SendEventMessage(m_hovered_widget, EventMessageType::PointerEnter);
            }
        } else {
            if (m_hovered_widget) {
                // Send exit to previous hovered widget.
                SendEventMessage(m_hovered_widget, EventMessageType::PointerExit);
                m_hovered_widget = nullptr;
            }
        }

        if (m_hovered_widget) {
            if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
                SendEventMessage(m_hovered_widget, EventMessageType::PointerClick);
            }
        }
    }

    //--------------------------------------------------------------
    void Canvas::OnLateUpdate(float32 delta_time) {
        UpdateScale();

        for (Widget *widget : m_widgets) {
            if (widget->IsDirty()) {
                m_mesh_builder.Clear();
                widget->OnRebuildMesh(m_mesh_builder);
                widget->ResetDirty();
            }
        }
    }

    //--------------------------------------------------------------
    void Canvas::UpdateScale() {
        if (Display::GetWidth() != m_cached_display_width || Display::GetHeight() != m_cached_display_height) {
            float32 display_width = static_cast<float32>(Display::GetWidth());
            float32 display_height = static_cast<float32>(Display::GetHeight());
            m_cached_display_width = Display::GetWidth();
            m_cached_display_height = Display::GetHeight();

            switch (m_scale_mode) {
                case CanvasScaleMode::ScaleWithScreenSize:
                {
                    float32 log_base = 2;
                    float32 log_width = Math::Log(display_width / m_reference_resolution.x) / Math::Log(log_base);
                    float32 log_height = Math::Log(display_height / m_reference_resolution.y) / Math::Log(log_base);
                    float32 log_weighted_average = Math::Lerp(log_width, log_height, 0.5f);
                    float32 computed_scale = Math::Pow(log_base, log_weighted_average);
                    m_scale = computed_scale;
                    break;
                }
                case CanvasScaleMode::ConstantPixelSize:
                {
                    m_scale = m_scale_factor;
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }

            // Our actual size has to be determined based on the scale factor.
            float32 inverse_scale = 1.0f / m_scale;
            RectTransform *transform = GetEntity()->GetComponent<RectTransform>();
            HYP_ASSERT(transform);
            transform->SetSize(Vec2(inverse_scale * display_width, inverse_scale * display_height));
            transform->SetAnchoredPosition(Vec3(display_width / 2.0f, display_height / 2.0f, 0.0f));
            transform->SetLocalScale(Vec3(m_scale, m_scale, m_scale));
        }
    }

    //--------------------------------------------------------------
    Vector<Widget *> Canvas::RaycastWidgets(Vec2 screen_position) {
        Vector<Widget *> result;
        for (Widget *widget : m_widgets) {
            if (!RectTransformUtility::RectangleContainsScreenPoint(widget->GetRectTransform(), screen_position)) {
                continue;
            }

            if (!widget->IsRaycastTarget()) {
                continue;
            }

            result.push_back(widget);
        }

        std::sort(result.begin(), result.end(), [](Widget *lhs, Widget *rhs) {
            return lhs->GetDepth() > rhs->GetDepth();
        });

        return result;
    }

    //--------------------------------------------------------------
    void Canvas::SendEventMessage(Widget *widget, EventMessageType type) {
        widget->OnEventMessage({ type });
    }

    //--------------------------------------------------------------
    void Canvas::RegisterWidget(Widget *widget) {
        HYP_ASSERT(std::find(m_widgets.begin(), m_widgets.end(), widget) == m_widgets.end());

        m_widgets.push_back(widget);

        UpdateWidgetDepths();
    }

    //--------------------------------------------------------------
    void Canvas::UnregisterWidget(Widget *widget) {
        HYP_ASSERT(std::find(m_widgets.begin(), m_widgets.end(), widget) != m_widgets.end());

        auto begin = m_widgets.begin();
        auto end = m_widgets.end();
        if (std::find(begin, end, widget) != end) {
            m_widgets.erase(std::remove(begin, end, widget));
        }

        UpdateWidgetDepths();
    }

    //--------------------------------------------------------------
    void Canvas::UpdateWidgetDepths() {
        Vector<Widget *> widgets = GetEntity()->GetComponentsInChildren<Widget>();
        int32 depth = 0;
        for (Widget *widget : widgets) {
            widget->m_depth = depth++;
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Canvas)
    HYP_REFLECT_BASE(Component)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Canvas(); })
    HYP_REFLECT_METADATA_POINTER(MetadataKind::RequireComponent, Type::Get<RectTransform>())
    HYP_REFLECT_END()

}