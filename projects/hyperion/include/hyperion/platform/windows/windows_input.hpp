#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/gamepad_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class WindowsWindow;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsInput : public InputImplementation {
    public:
        WindowsInput();
        ~WindowsInput();
    public:
        bool IsKeyDown(KeyCode key_code) const override {
            return m_keys_down[static_cast<int32>(key_code)];
        }
        bool IsKeyHold(KeyCode key_code) const override { return m_keys[static_cast<int32>(key_code)]; }
        bool IsKeyUp(KeyCode key_code) const override { return m_keys_up[static_cast<int32>(key_code)]; }

        Vector2 GetMousePosition() const override { return m_mouse_position; }
        float32 GetMouseScroll() const override { return m_mouse_scroll; }

        bool IsMouseButtonDown(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_down[static_cast<int32>(mouse_button_code)]; }
        bool IsMouseButtonHold(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons[static_cast<int32>(mouse_button_code)]; }
        bool IsMouseButtonUp(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_up[static_cast<int32>(mouse_button_code)]; }

        const List<Gamepad> &GetConnectedGamepads() const override { return m_gamepads_connected; }
        Vector2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const override { return m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(axis)]; }
        void SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) override;

        float32 GetGamepadDeadZone() const override { return m_gamepad_dead_zone; }
        void SetGamepadDeadZone(float32 gamepad_dead_zone) { m_gamepad_dead_zone = gamepad_dead_zone; }

        bool IsGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons_down[static_cast<int32>(gamepad_button_code)]; }
        bool IsGamepadButtonHold(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons[static_cast<int32>(gamepad_button_code)]; }
        bool IsGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons_up[static_cast<int32>(gamepad_button_code)]; }
    private:
        void OnEvent(Event &event);
        void Update();
        void Reset();

        void QueryConnectedGamepads();

        void OnKeyEvent(KeyEvent &event, bool down);
        void OnMouseButtonEvent(MouseButtonEvent &event, bool down);

        void HandleGamepadButtonCode(Gamepad gamepad, GamepadButtonCode button_code, bool down);
        Gamepad GetGamepadFromId(uint32 id);
        uint32 GetIdFromGamepad(Gamepad gamepad);
        Vector2 ApplyGamepadDeadzone(float32 x, float32 y);
    private:
        bool m_keys_down[static_cast<int32>(KeyCode::Last)];
        bool m_keys[static_cast<int32>(KeyCode::Last)];
        bool m_keys_last[static_cast<int32>(KeyCode::Last)];
        bool m_keys_up[static_cast<int32>(KeyCode::Last)];

        bool m_mouse_buttons_down[static_cast<int32>(MouseButtonCode::Last)];
        bool m_mouse_buttons[static_cast<int32>(MouseButtonCode::Last)];
        bool m_mouse_buttons_last[static_cast<int32>(MouseButtonCode::Last)];
        bool m_mouse_buttons_up[static_cast<int32>(MouseButtonCode::Last)];

        Vector2 m_mouse_position;
        float32 m_mouse_scroll;

        struct GamepadState {
            bool buttons_down[static_cast<int32>(GamepadButtonCode::Last)];
            bool buttons[static_cast<int32>(GamepadButtonCode::Last)];
            bool buttons_last[static_cast<int32>(GamepadButtonCode::Last)];
            bool buttons_up[static_cast<int32>(GamepadButtonCode::Last)];

            Vector2 axes[static_cast<int32>(GamepadAxis::Last)];
        };

        bool m_gamepad_input_active = true;
        List<Gamepad> m_gamepads_connected;
        GamepadState m_gamepads[static_cast<int32>(Gamepad::Last)];
        float32 m_gamepad_dead_zone = 0.25f;
    private:
        friend class Hyperion::WindowsWindow;
    };

}