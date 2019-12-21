#pragma once

#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {

    class KeyEvent : public Event {
    private:
        KeyCode m_key_code;
        KeyModifier m_key_modifier;
    public:
        KeyEvent(KeyCode key_code, KeyModifier key_modifier) : m_key_code(key_code), m_key_modifier(key_modifier) {}

        inline KeyCode GetKeyCode() const { return m_key_code; }
        inline bool HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Keyboard; }
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode key_code, KeyModifier key_modifier) : KeyEvent(key_code, key_modifier) {}

        EventType GetType() const override { return EventType::KeyPressed; }

        static EventType GetStaticType() { return EventType::KeyPressed; }
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(KeyCode key_code, KeyModifier key_modifier) : KeyEvent(key_code, key_modifier) {}

        EventType GetType() const override { return EventType::KeyReleased; }

        static EventType GetStaticType() { return EventType::KeyReleased; }
    };

    class KeyTypedEvent : public Event {
    private:
        u32 m_character;
        KeyModifier m_key_modifier;
    public:
        KeyTypedEvent(u32 character, KeyModifier key_modifier) : m_character(character), m_key_modifier(key_modifier) {}

        inline u32 GetCharacter() const { return m_character; }
        inline bool HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EventType GetType() const override { return EventType::KeyTyped; }
        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Keyboard; }

        static EventType GetStaticType() { return EventType::KeyTyped; }
    };

}