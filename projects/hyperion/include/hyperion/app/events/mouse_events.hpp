#pragma once

#include "hyperion/app/events/event.hpp"

namespace Hyperion {

    class MouseButtonEvent : public Event {
    private:
        MouseButtonCode m_mouse_button_code;
        KeyModifier m_key_modifier;
    public:
        MouseButtonEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : m_mouse_button_code(mouse_button_code), m_key_modifier(key_modifier) {}

        inline MouseButtonCode GetMouseButtonCode() const { return m_mouse_button_code; }
        inline bool HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Mouse & EventCategory::MouseButton; }
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : MouseButtonEvent(mouse_button_code, key_modifier) {}

        EventType GetType() const override { return EventType::MouseButtonPressed; }

        static EventType GetStaticType() { return EventType::MouseButtonPressed; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : MouseButtonEvent(mouse_button_code, key_modifier) {}

        EventType GetType() const override { return EventType::MouseButtonReleased; }

        static EventType GetStaticType() { return EventType::MouseButtonReleased; }
    };

    class MouseMovedEvent : public Event {
    private:
        f32 m_x;
        f32 m_y;
    public:
        MouseMovedEvent(f32 x, f32 y) : m_x(x), m_y(y) {}

        inline f32 GetX() const { return m_x; }
        inline f32 GetY() const { return m_y; }

        EventType GetType() const override { return EventType::MouseMoved; }
        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Mouse; }

        static EventType GetStaticType() { return EventType::MouseMoved; }
    };

    class MouseScrolledEvent : public Event {
    private:
        f32 m_scroll;
    public:
        MouseScrolledEvent(f32 scroll) : m_scroll(scroll) {}

        inline f32 GetScroll() const { return m_scroll; }

        EventType GetType() const override { return EventType::MouseScrolled; }
        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Mouse; }

        static EventType GetStaticType() { return EventType::MouseScrolled; }
    };

}