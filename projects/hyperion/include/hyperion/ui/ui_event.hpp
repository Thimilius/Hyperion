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

        inline KeyCode GetKeyCode() const { return m_key_code; }
        inline KeyModifier GetKeyModifier() const { return m_key_modifier; }
        inline uint32 GetKeyTyped() const { return m_key_typed; }
    private:
        UIEventType m_type;

        Vector2Int m_pointer_position;
        float32 m_pointer_scroll = 0;
         
        KeyCode m_key_code = KeyCode::None;
        KeyModifier m_key_modifier = KeyModifier::None;
        uint32 m_key_typed = 0;
    private:
        friend class Hyperion::UI::UIEventSystem;
    };

}