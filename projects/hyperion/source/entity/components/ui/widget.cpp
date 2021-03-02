//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/widget.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/ui/ui_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Widget::SetColor(Color color) {
        if (m_color == color) {
            return;
        }

        m_color = color;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Widget::OnMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::TransformChanged: {
                SetDirty();
                break;
            }
            case EntityMessageType::TransformParentChanged: {
                m_canvas->UnregisterWidget(this);

                m_canvas = GetEntity()->GetComponentInParent<Canvas>();
                HYP_ASSERT(m_canvas);

                m_canvas->RegisterWidget(this);
                break;
            }
        }
    }

    //--------------------------------------------------------------
    void Widget::OnCreate() {
        Behaviour::OnCreate();

        GetEntity()->RegisterMessageListener(this);
        UiEngine::RegisterWidget(this);

        m_rect_transform = GetEntity()->GetComponent<RectTransform>();
        HYP_ASSERT(m_rect_transform);
        m_widget_renderer = GetEntity()->GetComponent<WidgetRenderer>();
        HYP_ASSERT(m_widget_renderer);
        m_canvas = GetEntity()->GetComponentInParent<Canvas>();
        HYP_ASSERT(m_canvas);

        m_canvas->RegisterWidget(this);
    }

    //--------------------------------------------------------------
    void Widget::OnDestroy() {
        GetEntity()->UnregisterMessageListener(this);
        UiEngine::UnregisterWidget(this);

        Behaviour::OnDestroy();
    }

    //--------------------------------------------------------------
    void Widget::OnEventMessage(EventMessage message) {
        switch (message.type) {
            case EventMessageType::PointerEnter: HYP_TRACE("{} - PointerEnter", GetEntity()->GetName()); break;
            case EventMessageType::PointerExit: HYP_TRACE("{} - PointerExit", GetEntity()->GetName()); break;
            case EventMessageType::PointerClick: HYP_TRACE("{} - PointerClick", GetEntity()->GetName()); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}