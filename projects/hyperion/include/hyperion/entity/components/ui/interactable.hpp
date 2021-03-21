#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Interactable : public Widget {
        HYP_REFLECT();
    public:
        inline bool IsInteractable() const { return m_is_interactable; }
        inline void SetIsInteractable(bool is_interactable) { m_is_interactable = is_interactable; }

        inline Widget *GetTargetWidget() const { return m_target_widget; }
        inline void SetTargetWidget(Widget *target_widget) { m_target_widget = target_widget; }
    protected:
        Interactable() = default;
        Interactable(const String &name) : Widget(name) { }
    protected:
        void SetTargetColor(Color color);
    private:
        bool m_is_interactable = true;
        Widget *m_target_widget = nullptr;
    };

}