#pragma once

#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {

    class GamepadEvent : public Event {
    private:
        Gamepad m_gamepad;
    public:
        GamepadEvent(Gamepad gamepad) : m_gamepad(gamepad) { }

        inline Gamepad GetGamepad() const { return m_gamepad; }

        EventCategory GetCategory() const override { return EventCategory::Input & EventCategory::Gamepad; }
    };

    class GamepadConnectionChangedEvent : public GamepadEvent {
    private:
        bool m_connected;
    public:
        GamepadConnectionChangedEvent(Gamepad gamepad, bool connected) : GamepadEvent(gamepad), m_connected(connected) { }

        inline bool GetConnected() const { return m_connected; }

        EventType GetType() const override { return EventType::GamepadConnectionChanged; }

        static EventType GetStaticType() { return EventType::GamepadConnectionChanged; }
    };

}