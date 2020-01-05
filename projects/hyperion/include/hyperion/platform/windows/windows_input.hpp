#pragma once

#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/core/app/events/gamepad_events.hpp"

namespace Hyperion {

    class WindowsInput : public InputImplementation {
    private:
        bool m_keys_down[(s32)KeyCode::Last];
        bool m_keys[(s32)KeyCode::Last];
        bool m_keys_last[(s32)KeyCode::Last];
        bool m_keys_up[(s32)KeyCode::Last];

        bool m_mouse_buttons_down[(s32)MouseButtonCode::Last];
        bool m_mouse_buttons[(s32)MouseButtonCode::Last];
        bool m_mouse_buttons_last[(s32)MouseButtonCode::Last];
        bool m_mouse_buttons_up[(s32)MouseButtonCode::Last];

        Vec2 m_mouse_position;
        f32 m_mouse_scroll;

        struct GamepadState {
            bool buttons_down[(s32)GamepadButtonCode::Last];
            bool buttons[(s32)GamepadButtonCode::Last];
            bool buttons_last[(s32)GamepadButtonCode::Last];
            bool buttons_up[(s32)GamepadButtonCode::Last];

            Vec2 axes[(s32)GamepadAxis::Last];
        };

        inline static Vector<Gamepad> m_gamepads_connected;
        inline static GamepadState m_gamepads[(s32)Gamepad::Last];
    public:
        bool GetKeyDown(KeyCode key_code) const override { return m_keys_down[(s32)key_code]; }
        bool GetKey(KeyCode key_code) const override { return m_keys[(s32)key_code]; }
        bool GetKeyUp(KeyCode key_code) const override { return m_keys_up[(s32)key_code]; }

        Vec2 GetMousePosition() const override { return m_mouse_position; }
        f32 GetMouseScroll() const override { return m_mouse_scroll; }

        bool GetMouseButtonDown(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_down[(s32)mouse_button_code]; }
        bool GetMouseButton(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons[(s32)mouse_button_code]; }
        bool GetMouseButtonUp(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_up[(s32)mouse_button_code]; }

        const Vector<Gamepad> &GetConnectedGamepads() const override { return m_gamepads_connected; }
        Vec2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const override { return m_gamepads[(s32)gamepad].axes[(s32)axis]; }

        bool GetGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[(s32)gamepad].buttons_down[(s32)gamepad_button_code]; }
        bool GetGamepadButton(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[(s32)gamepad].buttons[(s32)gamepad_button_code]; }
        bool GetGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[(s32)gamepad].buttons_up[(s32)gamepad_button_code]; }
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

        friend class WindowsWindow;
    };

}