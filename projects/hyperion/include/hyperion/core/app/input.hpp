#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/vec2.hpp"

namespace Hyperion {

    enum class KeyCode {
        None,

        Alpha0,
        Alpha1,
        Alpha2,
        Alpha3,
        Alpha4,
        Alpha5,
        Alpha6,
        Alpha7,
        Alpha8,
        Alpha9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        Escape,
        Back,
        Tab,
        Return,
        Space,

        Print,
        Scroll,
        Pause,

        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        Plus,
        Minus,
        Period,
        Comma,

        Up,
        Down,
        Left,
        Right,

        Numlock,
        Capslock,

        LeftSuper,
        RightSuper,
        Application,

        LeftControl,
        RightControl,

        LeftShift,
        RightShift,

        LeftAlt,
        RightAlt,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        NumAdd,
        NumSubtract,
        NumDivide,
        NumMultiply,
        NumEnter,
        NumComma,

        Last
    };

    enum class MouseButtonCode {
        None,

        Left,
        Right,
        Middle,

        Button4,
        Button5,

        Last
    };

    enum class KeyModifier {
        None     = 0,
        Shift    = BIT(0),
        Control  = BIT(1),
        Alt      = BIT(2),
        Super    = BIT(3),
        Capslock = BIT(4),
        Numlock  = BIT(5)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(KeyModifier)

    enum class Gamepad {
        Gamepad1,
        Gamepad2,
        Gamepad3,
        Gamepad4,

        Last
    };

    enum class GamepadButtonCode {
        Start,
        Back,

        A,
        B,
        X,
        Y,

        DpadLeft,
        DpadRight,
        DpadUp,
        DpadDown,

        LeftThumb,
        RightThumb,

        LeftShoulder,
        RightShoulder,

        Last
    };

    enum class GamepadAxis {
        LeftStick,
        RightStick,

        LeftTrigger,
        RightTrigger,

        Last
    };

    class Input {
    private:
        inline static bool s_keys_down[(s32)KeyCode::Last];
        inline static bool s_keys[(s32)KeyCode::Last];
        inline static bool s_keys_last[(s32)KeyCode::Last];
        inline static bool s_keys_up[(s32)KeyCode::Last];

        inline static bool s_mouse_buttons_down[(s32)MouseButtonCode::Last];
        inline static bool s_mouse_buttons[(s32)MouseButtonCode::Last];
        inline static bool s_mouse_buttons_last[(s32)MouseButtonCode::Last];
        inline static bool s_mouse_buttons_up[(s32)MouseButtonCode::Last];
        
        struct GamepadState {
            bool buttons_down[(s32)GamepadButtonCode::Last];
            bool buttons[(s32)GamepadButtonCode::Last];
            bool buttons_last[(s32)GamepadButtonCode::Last];
            bool buttons_up[(s32)GamepadButtonCode::Last];
        };

        inline static Vector<Gamepad> s_gamepads_connected;
        inline static GamepadState s_gamepads[(s32)Gamepad::Last];

        inline static Vec2 s_mouse_position;
        inline static f32 s_mouse_scroll;
    public:
        inline static bool GetKeyDown(KeyCode key_code) { return s_keys_down[(s32)key_code]; }
        inline static bool GetKey(KeyCode key_code) { return s_keys[(s32)key_code]; }
        inline static bool GetKeyUp(KeyCode key_code) { return s_keys_up[(s32)key_code]; }

        inline static Vec2 GetMousePosition() { return s_mouse_position; }
        inline static f32 GetMouseScroll() { return s_mouse_scroll; }

        inline static bool GetMouseButtonDown(MouseButtonCode mouse_button_code) { return s_mouse_buttons_down[(s32)mouse_button_code]; }
        inline static bool GetMouseButton(MouseButtonCode mouse_button_code) { return s_mouse_buttons[(s32)mouse_button_code]; }
        inline static bool GetMouseButtonUp(MouseButtonCode mouse_button_code) { return s_mouse_buttons_up[(s32)mouse_button_code]; }

        inline static const Vector<Gamepad> &GetConnectedGamepads() { return s_gamepads_connected; }

        inline static bool GetGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_gamepads[(s32)gamepad].buttons_down[(s32)gamepad_button_code]; }
        inline static bool GetGamepadButton(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_gamepads[(s32)gamepad].buttons[(s32)gamepad_button_code]; }
        inline static bool GetGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_gamepads[(s32)gamepad].buttons_up[(s32)gamepad_button_code]; }
    private:
        Input() = delete;
        ~Input() = delete;

        static void Update();
        static void Reset();

        friend class Application;
    };

}
