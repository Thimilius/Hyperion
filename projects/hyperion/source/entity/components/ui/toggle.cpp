//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/toggle.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Toggle::SetIsOn(bool is_on) {
        if (is_on != m_is_on) {
            m_is_on = is_on;

            if (m_callback != nullptr) {
                m_callback(is_on);
            }

            if (is_on) {
                SetTargetColor(Color(0.012f, 0.439f, 0.643f, 1.0f));
            } else {
                SetTargetColor(Color::White());
            }
        }
    }

    //--------------------------------------------------------------
    void Toggle::OnEventMessage(EventMessage event_message) {
        if (event_message.type == EventMessageType::PointerClick) {
            SetIsOn(!m_is_on);
        }
    }

    //--------------------------------------------------------------
    void Toggle::SetTargetColor(Color color) {
        if (m_target_widget != nullptr) {
            m_target_widget->GetWidgetRenderer()->SetColor(color);
        }
    }

    //--------------------------------------------------------------
    Toggle *Toggle::Create() {
        return new Toggle();
    }

}