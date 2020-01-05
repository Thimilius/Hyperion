#include "hyppch.hpp"

#include "hyperion/core/app/input.hpp"

namespace Hyperion {

    void Input::Update() {
        memset(&s_keys_down, false, sizeof(s_keys_down));
        memset(&s_keys_up, false, sizeof(s_keys_up));
        memcpy(&s_keys_last, &s_keys, sizeof(s_keys_last));

        memset(&s_mouse_buttons_down, false, sizeof(s_mouse_buttons_down));
        memset(&s_mouse_buttons_up, false, sizeof(s_mouse_buttons_up));
        memcpy(&s_mouse_buttons_last, &s_mouse_buttons, sizeof(s_mouse_buttons_last));

        for (u32 i = 0; i < (u32)Gamepad::Last; i++) {
            GamepadState &gamepad = s_gamepads[i];

            memset(&gamepad.buttons_down, false, sizeof(gamepad.buttons_down));
            memset(&gamepad.buttons_up, false, sizeof(gamepad.buttons_up));
            memcpy(&gamepad.buttons_last, &gamepad.buttons, sizeof(gamepad.buttons_last));
        }

        s_mouse_scroll = 0.0f;
    }

    void Input::Reset() {
        memset(&s_keys_down, false, sizeof(s_keys_down));
        memset(&s_keys_up, false, sizeof(s_keys_up));
        memset(&s_keys, false, sizeof(s_keys));
        memset(&s_keys_last, false, sizeof(s_keys_last));

        memset(&s_mouse_buttons_down, false, sizeof(s_mouse_buttons_down));
        memset(&s_mouse_buttons_up, false, sizeof(s_mouse_buttons_up));
        memset(&s_mouse_buttons, false, sizeof(s_mouse_buttons));
        memset(&s_mouse_buttons_last, false, sizeof(s_mouse_buttons_last));

        for (u32 i = 0; i < (u32)Gamepad::Last; i++) {
            GamepadState &gamepad = s_gamepads[i];

            memset(&gamepad.buttons_down, false, sizeof(gamepad.buttons_down));
            memset(&gamepad.buttons_up, false, sizeof(gamepad.buttons_up));
            memset(&gamepad.buttons, false, sizeof(gamepad.buttons));
            memset(&gamepad.buttons_last, false, sizeof(gamepad.buttons_last));
        }

        s_mouse_scroll = 0.0f;
    }

}