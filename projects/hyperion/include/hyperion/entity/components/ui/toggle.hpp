#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/interactable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using OnIsOnChangedCallback = std::function<void(bool)>;

    class Toggle : public Interactable {
        HYP_REFLECT(Interactable);
    public:
        inline bool IsOn() const { return m_is_on; }
        void SetIsOn(bool is_on);

        inline Widget *GetTargetWidget() const { return m_target_widget; }
        inline void SetTargetWidget(Widget *target_widget) { m_target_widget = target_widget; }

        inline void SetIsOnChangedCallback(OnIsOnChangedCallback callback) { m_callback = callback; }
    protected:
        void OnEventMessage(EventMessage event_message) override;
    private:
        Toggle() : Interactable("Toggle") {}
    private:
        void SetTargetColor(Color color);
    private:
        static Toggle *Create();
    private:
        bool m_is_on = false;

        Widget *m_target_widget = nullptr;

        OnIsOnChangedCallback m_callback = nullptr;
    };

}