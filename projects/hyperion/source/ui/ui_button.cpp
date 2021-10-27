//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_button.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    UIButton::UIButton() {
        SetRaycastTarget(true);
    }

    //--------------------------------------------------------------
    void UIButton::Click() {
        if (IsInteractable()) {
            if (m_click_callback) {
                m_click_callback();
            }
        }
    }

    //--------------------------------------------------------------
    void UIButton::RegisterClickCallback(const UIButtonClickCallback &callback) {
        m_click_callback = callback;
    }

    //--------------------------------------------------------------
    void UIButton::UnregisterClickCallback() {
        m_click_callback = UIButtonClickCallback();
    }

    //--------------------------------------------------------------
    void UIButton::OnEvent(UIEvent &event) {
        UIElement::OnEvent(event);

        if (event.GetType() == UIEventType::PointerClick) {
            Click();
        }
    }

}