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
        inline bool HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        AppEventCategory GetCategory() const override { return AppEventCategory::Input & AppEventCategory::Keyboard; }
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

    class KeyTypedAppEvent : public AppEvent {
    public:
        KeyTypedAppEvent(uint32 character, KeyModifier key_modifier) : m_character(character), m_key_modifier(key_modifier) { }

        inline uint32 GetCharacter() const { return m_character; }
        inline bool HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        AppEventType GetType() const override { return AppEventType::KeyTyped; }
        AppEventCategory GetCategory() const override { return AppEventCategory::Input & AppEventCategory::Keyboard; }
    public:
        static AppEventType GetStaticType() { return AppEventType::KeyTyped; }
    private:
        uint32 m_character;
        KeyModifier m_key_modifier;
    };

}