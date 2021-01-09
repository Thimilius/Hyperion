#pragma once

#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/core/app/events/gamepad_events.hpp"

namespace Hyperion {
    class WindowsWindow;
}

namespace Hyperion {

    class WindowsInput : public InputImplementation {
    public:
        WindowsInput();
        ~WindowsInput();

        bool GetKeyDown(KeyCode key_code) const override { return m_keys_down[static_cast<s32>(key_code)]; }
        bool GetKey(KeyCode key_code) const override { return m_keys[static_cast<s32>(key_code)]; }
        bool GetKeyUp(KeyCode key_code) const override { return m_keys_up[static_cast<s32>(key_code)]; }

        Vec2 GetMousePosition() const override { return m_mouse_position; }
        f32 GetMouseScroll() const override { return m_mouse_scroll; }

        bool GetMouseButtonDown(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_down[static_cast<s32>(mouse_button_code)]; }
        bool GetMouseButton(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons[static_cast<s32>(mouse_button_code)]; }
        bool GetMouseButtonUp(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_up[static_cast<s32>(mouse_button_code)]; }

        const Vector<Gamepad> &GetConnectedGamepads() const override { return m_gamepads_connected; }
        Vec2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const override { return m_gamepads[static_cast<s32>(gamepad)].axes[static_cast<s32>(axis)]; }
        void SetGamepadVibration(Gamepad gamepad, f32 left_vibration, f32 right_vibration) override;

        f32 GetGamepadDeadZone() const override { return m_gamepad_dead_zone; }
        void SetGamepadDeadZone(f32 gamepad_dead_zone) { m_gamepad_dead_zone = gamepad_dead_zone; }

        bool GetGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<s32>(gamepad)].buttons_down[static_cast<s32>(gamepad_button_code)]; }
        bool GetGamepadButton(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<s32>(gamepad)].buttons[static_cast<s32>(gamepad_button_code)]; }
        bool GetGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<s32>(gamepad)].buttons_up[static_cast<s32>(gamepad_button_code)]; }
    private:
        void OnEvent(Event &event);
        void Update();
        void Reset();

        void QueryConnectedGamepads();

        void OnKeyEvent(KeyEvent &event, bool down);
        void OnMouseButtonEvent(MouseButtonEvent &event, bool down);

        void HandleGamepadButtonCode(Gamepad gamepad, GamepadButtonCode button_code, bool down);
        Gamepad GetGamepadFromId(u32 id);
        u32 GetIdFromGamepad(Gamepad gamepad);
        Vec2 ApplyGamepadDeadzone(f32 x, f32 y);
    private:
        bool m_keys_down[static_cast<s32>(KeyCode::Last)];
        bool m_keys[static_cast<s32>(KeyCode::Last)];
        bool m_keys_last[static_cast<s32>(KeyCode::Last)];
        bool m_keys_up[static_cast<s32>(KeyCode::Last)];

        bool m_mouse_buttons_down[static_cast<s32>(MouseButtonCode::Last)];
        bool m_mouse_buttons[static_cast<s32>(MouseButtonCode::Last)];
        bool m_mouse_buttons_last[static_cast<s32>(MouseButtonCode::Last)];
        bool m_mouse_buttons_up[static_cast<s32>(MouseButtonCode::Last)];

        Vec2 m_mouse_position;
        f32 m_mouse_scroll;

        struct GamepadState {
            bool buttons_down[static_cast<s32>(GamepadButtonCode::Last)];
            bool buttons[static_cast<s32>(GamepadButtonCode::Last)];
            bool buttons_last[static_cast<s32>(GamepadButtonCode::Last)];
            bool buttons_up[static_cast<s32>(GamepadButtonCode::Last)];

            Vec2 axes[static_cast<s32>(GamepadAxis::Last)];
        };

        bool m_gamepad_input_active = true;
        Vector<Gamepad> m_gamepads_connected;
        GamepadState m_gamepads[static_cast<s32>(Gamepad::Last)];
        f32 m_gamepad_dead_zone = 0.25f;
    private:
        friend class Hyperion::WindowsWindow;
    };

}