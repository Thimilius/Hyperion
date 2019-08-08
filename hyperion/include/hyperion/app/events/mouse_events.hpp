#pragma once

#include "event.hpp"

namespace Hyperion::Events {

    class CMouseButtonEvent : public CEvent {
    private:
        EMouseButtonCode m_mouse_button_code;
        EKeyModifier m_key_modifier;
    public:
        CMouseButtonEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : m_mouse_button_code(mouse_button_code), m_key_modifier(key_modifier) {}

        inline EMouseButtonCode GetMouseButtonCode() const { return m_mouse_button_code; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse & EEventCategory::MouseButton; }
    };

    class CMouseButtonPressedEvent : public CMouseButtonEvent {
    public:
        CMouseButtonPressedEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : CMouseButtonEvent(mouse_button_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::MouseButtonPressed; }

        static EEventType GetStaticType() { return EEventType::MouseButtonPressed; }
    };

    class CMouseButtonReleasedEvent : public CMouseButtonEvent {
    public:
        CMouseButtonReleasedEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : CMouseButtonEvent(mouse_button_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::MouseButtonReleased; }

        static EEventType GetStaticType() { return EEventType::MouseButtonReleased; }
    };

    class CMouseMovedEvent : public CEvent {
    private:
        float m_x;
        float m_y;
    public:
        CMouseMovedEvent(float x, float y) : m_x(x), m_y(y) {}

        inline float GetX() const { return m_x; }
        inline float GetY() const { return m_y; }

        EEventType GetType() const override { return EEventType::MouseMoved; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse; }

        static EEventType GetStaticType() { return EEventType::MouseMoved; }
    };

    class CMouseScrolledEvent : public CEvent {
    private:
        float m_scroll;
    public:
        CMouseScrolledEvent(float scroll) : m_scroll(scroll) {}

        inline float GetScroll() const { return m_scroll; }

        EEventType GetType() const override { return EEventType::MouseScrolled; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse; }

        static EEventType GetStaticType() { return EEventType::MouseScrolled; }
    };

}