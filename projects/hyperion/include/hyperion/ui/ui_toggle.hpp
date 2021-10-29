#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ui/ui_element.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    using UIToggleCallback = std::function<void(bool8)>;

    class UIToggle : public UIElement {
        HYP_OBJECT(UIToggle);
    public:
        UIToggle();
    public:
        inline bool8 IsOn() const { return m_is_on; }
        void SetIsOn(bool8 is_on);
        void Toggle();

        inline UIElement *GetToggleGraphic() const { return m_toggle_graphic; }
        void SetToggleGraphic(UIElement *toggle_graphic);
        inline Color GetToggleOnColor() const { return m_toggle_on_color; }
        void SetToggleOnColor(Color toggle_on_color);
        inline Color GetToggleOffColor() const { return m_toggle_off_color; }
        void SetToggleOffColor(Color toggle_off_color);

        void RegisterToggleCallback(const UIToggleCallback &callback);
        void UnregisterToggleCallback();

        virtual void OnEvent(UIEvent &event) override;
    private:
        void StateTransition();
    private:
        bool8 m_is_on = true;
        UIElement *m_toggle_graphic = nullptr;
        Color m_toggle_on_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
        Color m_toggle_off_color = Color(1.0f, 1.0f, 1.0f, 0.0f);
        UIToggleCallback m_toggle_callback;
    };

}