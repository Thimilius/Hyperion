#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/interactable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using OnClickCallback = std::function<void()>;

    class Button : public Interactable {
        HYP_REFLECT(Interactable);
    public:
        inline void SetClickCallback(OnClickCallback callback) { m_callback = callback; }

        void Click();
    protected:
        void OnEventMessage(EventMessage event_message) override;
    private:
        Button() : Interactable("Button") { }
    private:
        static Button *Create();
    private:
        OnClickCallback m_callback = nullptr;
    };

}