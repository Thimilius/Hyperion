#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/interactable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using OnIsOnChangedCallback = std::function<void(bool)>;

    class Toggle : public Interactable {
        HYP_REFLECT();
    public:
        inline bool IsOn() const { return m_is_on; }
        void SetIsOn(bool is_on);

        inline void SetIsOnChangedCallback(OnIsOnChangedCallback callback) { m_callback = callback; }
    protected:
        void OnEventMessage(EventMessage event_message) override;
    private:
        Toggle() : Interactable("Toggle") { }
    private:
        bool m_is_on = false;

        Widget *m_target_widget = nullptr;

        OnIsOnChangedCallback m_callback = nullptr;
    };

}