#pragma once

#include "hyperion/core/app/events/event.hpp"
#include "hyperion/core/app/input.hpp"

namespace Hyperion {

    class GamepadEvent : public Event {
    public:
        GamepadEvent(Gamepad gamepad) : m_gamepad(gamepad) { }

        inline Gamepad GetGamepad() const { return m_gamepad; }

        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Gamepad; }
    private:
        Gamepad m_gamepad;
    };

    class GamepadButtonEvent : public GamepadEvent {
    public:
        GamepadButtonEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadEvent(gamepad), m_button_code(button_code) { }

        inline GamepadButtonCode GetGamepadButtonCode() const { return m_button_code; }
    private:
        GamepadButtonCode m_button_code;
    };

    class GamepadButtonPressedEvent : public GamepadButtonEvent {
    public:
        GamepadButtonPressedEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadButtonEvent(gamepad, button_code) { }

        EventType GetType() const override { return EventType::GamepadButtonPressed; }

        static EventType GetStaticType() { return EventType::GamepadButtonPressed; }
    };

    class GamepadButtonReleasedEvent : public GamepadButtonEvent {
    public:
        GamepadButtonReleasedEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadButtonEvent(gamepad, button_code) {}

        EventType GetType() const override { return EventType::GamepadButtonReleased; }

        static EventType GetStaticType() { return EventType::GamepadButtonReleased; }
    };

    class GamepadConnectionChangedEvent : public GamepadEvent {
    public:
        GamepadConnectionChangedEvent(Gamepad gamepad, bool connected) : GamepadEvent(gamepad), m_connected(connected) { }

        inline bool GetConnected() const { return m_connected; }

        EventType GetType() const override { return EventType::GamepadConnectionChanged; }

        static EventType GetStaticType() { return EventType::GamepadConnectionChanged; }
    private:
        bool m_connected;
    };

}