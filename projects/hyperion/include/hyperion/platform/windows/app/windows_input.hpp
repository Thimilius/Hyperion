//------------------------ Header Guard ------------------------
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
        bool8 IsKeyDown(KeyCode key_code) const override { return m_keys_down[static_cast<int32>(key_code)]; }
        bool8 IsKeyHold(KeyCode key_code) const override { return m_keys[static_cast<int32>(key_code)]; }
        bool8 IsKeyUp(KeyCode key_code) const override { return m_keys_up[static_cast<int32>(key_code)]; }
        const Array<String> &GetKeysTyped() const override { return m_keys_typed; }

        Vector2Int GetMousePosition() const override { return m_mouse_position; }
        bool8 HasMouseMoved() const override { return m_last_mouse_position != m_mouse_position; }
        float32 GetMouseScroll() const override { return m_mouse_scroll; }
        bool8 HasMouseScrolled() const override { return m_mouse_scroll != 0.0f; }

        bool8 IsMouseButtonDown(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_down[static_cast<int32>(mouse_button_code)]; }
        bool8 IsMouseButtonHold(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons[static_cast<int32>(mouse_button_code)]; }
        bool8 IsMouseButtonUp(MouseButtonCode mouse_button_code) const override { return m_mouse_buttons_up[static_cast<int32>(mouse_button_code)]; }

        const Array<Gamepad> &GetConnectedGamepads() const override { return m_gamepads_connected; }
        Vector2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const override { return m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(axis)]; }
        void SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) override;

        float32 GetGamepadDeadZone() const override { return m_gamepad_dead_zone; }
        void SetGamepadDeadZone(float32 gamepad_dead_zone) { m_gamepad_dead_zone = gamepad_dead_zone; }

        bool8 IsGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons_down[static_cast<int32>(gamepad_button_code)]; }
        bool8 IsGamepadButtonHold(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons[static_cast<int32>(gamepad_button_code)]; }
        bool8 IsGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const override { return m_gamepads[static_cast<int32>(gamepad)].buttons_up[static_cast<int32>(gamepad_button_code)]; }

        inline const Array<AppEvent *> &GetEvents() const override { return m_events; }
    private:
        void OnAppEvent(AppEvent &app_event);
        void Update();
        void Reset();
        
        void QueryConnectedGamepads();

        void OnKeyEvent(KeyAppEvent &key_app_event, bool8 down);
        void OnMouseButtonEvent(MouseButtonAppEvent &mouse_button_app_event, bool8 down);

        void HandleGamepadButtonCode(Gamepad gamepad, GamepadButtonCode button_code, bool8 down);
        Gamepad GetGamepadFromId(uint32 id);
        uint32 GetIdFromGamepad(Gamepad gamepad);
        Vector2 ApplyGamepadDeadzone(float32 x, float32 y);
    private:
        bool8 m_keys_down[static_cast<int32>(KeyCode::Last)];
        bool8 m_keys[static_cast<int32>(KeyCode::Last)];
        bool8 m_keys_last[static_cast<int32>(KeyCode::Last)];
        bool8 m_keys_up[static_cast<int32>(KeyCode::Last)];
        Array<String> m_keys_typed;

        bool8 m_mouse_buttons_down[static_cast<int32>(MouseButtonCode::Last)];
        bool8 m_mouse_buttons[static_cast<int32>(MouseButtonCode::Last)];
        bool8 m_mouse_buttons_last[static_cast<int32>(MouseButtonCode::Last)];
        bool8 m_mouse_buttons_up[static_cast<int32>(MouseButtonCode::Last)];

        Vector2Int m_mouse_position;
        Vector2Int m_last_mouse_position;
        float32 m_mouse_scroll;

        struct GamepadState {
            bool8 buttons_down[static_cast<int32>(GamepadButtonCode::Last)];
            bool8 buttons[static_cast<int32>(GamepadButtonCode::Last)];
            bool8 buttons_last[static_cast<int32>(GamepadButtonCode::Last)];
            bool8 buttons_up[static_cast<int32>(GamepadButtonCode::Last)];

            Vector2 axes[static_cast<int32>(GamepadAxis::Last)];
        };

        bool8 m_gamepad_input_active = true;
        Array<Gamepad> m_gamepads_connected;
        GamepadState m_gamepads[static_cast<int32>(Gamepad::Last)];
        float32 m_gamepad_dead_zone = 0.25f;

        Array<AppEvent *> m_events;
    private:
        friend class Hyperion::WindowsWindow;
    };

}