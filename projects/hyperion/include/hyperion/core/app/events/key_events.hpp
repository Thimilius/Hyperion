//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/app_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class KeyAppEvent : public AppEvent {
    public:
        KeyAppEvent(KeyCode key_code, KeyModifier key_modifier) : m_key_code(key_code), m_key_modifier(key_modifier) { }

        inline KeyCode GetKeyCode() const { return m_key_code; }
        inline KeyModifier GetKeyModifier() const { return m_key_modifier; }
        inline bool8 HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        AppEventCategory GetCategory() const override { return AppEventCategory::Input | AppEventCategory::Keyboard; }
    private:
        KeyCode m_key_code;
        KeyModifier m_key_modifier;
    };

    class KeyPressedAppEvent : public KeyAppEvent {
    public:
        KeyPressedAppEvent(KeyCode key_code, KeyModifier key_modifier) : KeyAppEvent(key_code, key_modifier) { }

        AppEventType GetType() const override { return AppEventType::KeyPressed; }
    public:
        static AppEventType GetStaticType() { return AppEventType::KeyPressed; }
    };

    class KeyReleasedAppEvent : public KeyAppEvent {
    public:
        KeyReleasedAppEvent(KeyCode key_code, KeyModifier key_modifier) : KeyAppEvent(key_code, key_modifier) { }

        AppEventType GetType() const override { return AppEventType::KeyReleased; }
    public:
        static AppEventType GetStaticType() { return AppEventType::KeyReleased; }
    };

    class KeyTypedAppEvent : public KeyAppEvent {
    public:
        KeyTypedAppEvent(uint32 character, KeyCode key_code, KeyModifier key_modifier) : KeyAppEvent(key_code, key_modifier), m_character(character) { }

        inline uint32 GetCharacter() const { return m_character; }
        
        AppEventType GetType() const override { return AppEventType::KeyTyped; }
    public:
        static AppEventType GetStaticType() { return AppEventType::KeyTyped; }
    private:
        uint32 m_character;
    };

}