//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/app_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class GamepadAppEvent : public AppEvent {
    public:
        GamepadAppEvent(Gamepad gamepad) : m_gamepad(gamepad) { }

        inline Gamepad GetGamepad() const { return m_gamepad; }

        AppEventCategory GetCategory() const override { return AppEventCategory::Input | AppEventCategory::Gamepad; }
    private:
        Gamepad m_gamepad;
    };

    class GamepadButtonAppEvent : public GamepadAppEvent {
    public:
        GamepadButtonAppEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadAppEvent(gamepad), m_button_code(button_code) { }

        inline GamepadButtonCode GetGamepadButtonCode() const { return m_button_code; }
    private:
        GamepadButtonCode m_button_code;
    };

    class GamepadButtonPressedAppEvent : public GamepadButtonAppEvent {
    public:
        GamepadButtonPressedAppEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadButtonAppEvent(gamepad, button_code) { }

        AppEventType GetType() const override { return AppEventType::GamepadButtonPressed; }
    public:
        static AppEventType GetStaticType() { return AppEventType::GamepadButtonPressed; }
    };

    class GamepadButtonReleasedAppEvent : public GamepadButtonAppEvent {
    public:
        GamepadButtonReleasedAppEvent(Gamepad gamepad, GamepadButtonCode button_code) : GamepadButtonAppEvent(gamepad, button_code) {}

        AppEventType GetType() const override { return AppEventType::GamepadButtonReleased; }
    public:
        static AppEventType GetStaticType() { return AppEventType::GamepadButtonReleased; }
    };

    class GamepadConnectionChangedAppEvent : public GamepadAppEvent {
    public:
        GamepadConnectionChangedAppEvent(Gamepad gamepad, bool8 connected) : GamepadAppEvent(gamepad), m_connected(connected) { }

        inline bool8 GetConnected() const { return m_connected; }

        AppEventType GetType() const override { return AppEventType::GamepadConnectionChanged; }
    public:
        static AppEventType GetStaticType() { return AppEventType::GamepadConnectionChanged; }
    private:
        bool8 m_connected;
    };

}