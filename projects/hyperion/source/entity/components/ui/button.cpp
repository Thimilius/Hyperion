//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/button.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Button::Click() {
        if (!IsInteractable()) {
            return;
        }

        if (m_callback != nullptr) {
            m_callback();
        }
    }

    //--------------------------------------------------------------
    void Button::OnEventMessage(EventMessage event_message) {
        Interactable::OnEventMessage(event_message);

        if (event_message.type == EventMessageType::PointerClick) {
            Click();
        }
    }

    //--------------------------------------------------------------
    Button *Button::Create() {
        return new Button();
    }

}