#pragma once

#include "hyperion/app/events/event.hpp"

namespace Hyperion::Events {

    class CKeyEvent : public CEvent {
    private:
        EKeyCode m_key_code;
        EKeyModifier m_key_modifier;
    public:
        CKeyEvent(EKeyCode key_code, EKeyModifier key_modifier) : m_key_code(key_code), m_key_modifier(key_modifier) {}

        inline EKeyCode GetKeyCode() const { return m_key_code; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Keyboard; }
    };

    class CKeyPressedEvent : public CKeyEvent {
    public:
        CKeyPressedEvent(EKeyCode key_code, EKeyModifier key_modifier) : CKeyEvent(key_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::KeyPressed; }

        static EEventType GetStaticType() { return EEventType::KeyPressed; }
    };

    class CKeyReleasedEvent : public CKeyEvent {
    public:
        CKeyReleasedEvent(EKeyCode key_code, EKeyModifier key_modifier) : CKeyEvent(key_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::KeyReleased; }

        static EEventType GetStaticType() { return EEventType::KeyReleased; }
    };

    class CKeyTypedEvent : public CEvent {
    private:
        u32 m_character;
        EKeyModifier m_key_modifier;
    public:
        CKeyTypedEvent(u32 character, EKeyModifier key_modifier) : m_character(character), m_key_modifier(key_modifier) {}

        inline u32 GetCharacter() const { return m_character; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventType GetType() const override { return EEventType::KeyTyped; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Keyboard; }

        static EEventType GetStaticType() { return EEventType::KeyTyped; }
    };

}