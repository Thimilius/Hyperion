#pragma once

#include "hyperion/entity/components/ui/widget.hpp"

namespace Hyperion {

    class Interactable : public Widget {
        HYP_REFLECT(Widget);
    public:
        inline bool IsInteractable() const { return m_is_interactable; }
        inline void SetIsInteractable(bool is_interactable) { m_is_interactable = is_interactable; }
    protected:
        Interactable() = default;
        Interactable(const String &name) : Widget(name) { }
    private:
        bool m_is_interactable = true;
    };

}