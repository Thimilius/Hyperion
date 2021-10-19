#pragma once

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
        PointerClick
    };

    struct UIEvent {
    public:
        inline UIEventType GetType() const { return m_type; }

        inline float32 GetPointerScroll() const { return m_pointer_scroll; }
        inline Vector2Int GetPointerPosition() const { return m_pointer_position; }
    private:
        UIEventType m_type;

        Vector2Int m_pointer_position;
        float32 m_pointer_scroll;
    private:
        friend class Hyperion::UI::UIEventSystem;
    };

}