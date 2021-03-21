//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/toggle.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Toggle::SetIsOn(bool is_on) {
        if (!IsInteractable()) {
            return;
        }

        if (is_on != m_is_on) {
            m_is_on = is_on;

            if (is_on) {
                SetTargetColor(Color(0.012f, 0.439f, 0.643f, 1.0f));
            } else {
                SetTargetColor(Color::White());
            }

            if (m_callback != nullptr) {
                m_callback(is_on);
            }
        }
    }

    //--------------------------------------------------------------
    void Toggle::OnEventMessage(EventMessage event_message) {
        Interactable::OnEventMessage(event_message);

        if (event_message.type == EventMessageType::PointerClick) {
            SetIsOn(!m_is_on);
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Toggle)
    HYP_REFLECT_BASE(Interactable)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Toggle(); })
    HYP_REFLECT_END()

}