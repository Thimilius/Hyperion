//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
    class UIEventSystem;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    enum class UIEventType {
        PointerEnter,
        PointerExit,
        PointerDown,
        PointerUp,
        PointerMove,
        PointerScroll,
        PointerClick,

        Select,
        SelectUpdate,
        Deselect,

        KeyTyped,
    };

    struct UIEvent {
    public:
        inline UIEventType GetType() const { return m_type; }

        inline float32 GetPointerScroll() const { return m_pointer_scroll; }
        inline Vector2Int GetPointerPosition() const { return m_pointer_position; }

        inline uint32 GetKeyTyped() const { return m_key_typed; }
        inline KeyModifier GetKeyModifier() const { return m_key_modifier; }
    private:
        UIEventType m_type;

        Vector2Int m_pointer_position;
        float32 m_pointer_scroll = 0;

        uint32 m_key_typed = 0;
        KeyModifier m_key_modifier = KeyModifier::None;
    private:
        friend class Hyperion::UI::UIEventSystem;
    };

}