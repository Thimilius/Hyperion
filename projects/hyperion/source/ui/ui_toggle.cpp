//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_toggle.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    UIToggle::UIToggle() {
        SetRaycastTarget(true);
    }

    //--------------------------------------------------------------
    void UIToggle::SetIsOn(bool8 is_on) {
        if (m_is_on != is_on) {
            Toggle();
        }
    }

    //--------------------------------------------------------------
    void UIToggle::Toggle() {
        m_is_on = !m_is_on;

        StateTransition();

        if (m_toggle_callback) {
            m_toggle_callback(m_is_on);
        }
    }

    //--------------------------------------------------------------
    void UIToggle::SetToggleGraphic(UIElement *toggle_graphic) {
        m_toggle_graphic = toggle_graphic;

        StateTransition();
    }

    //--------------------------------------------------------------
    void UIToggle::SetToggleOnColor(Color toggle_on_color) {
        m_toggle_on_color = toggle_on_color;

        StateTransition();
    }

    //--------------------------------------------------------------
    void UIToggle::SetToggleOffColor(Color toggle_off_color) {
        m_toggle_off_color = toggle_off_color;

        StateTransition();
    }
    //--------------------------------------------------------------
    void UIToggle::RegisterClickCallback(const UIToggleCallback &callback) {
        m_toggle_callback = callback;
    }

    //--------------------------------------------------------------
    void UIToggle::UnregisterClickCallback() {
        m_toggle_callback = UIToggleCallback();
    }

    //--------------------------------------------------------------
    void UIToggle::OnEvent(UIEvent &event) {
        UIElement::OnEvent(event);

        if (event.GetType() == UIEventType::PointerClick) {
            Toggle();
        }
    }

    //--------------------------------------------------------------
    void UIToggle::StateTransition() {
        if (m_toggle_graphic) {
            m_toggle_graphic->GetRenderer().color = m_is_on ? m_toggle_on_color : m_toggle_off_color;
        }
    }

}