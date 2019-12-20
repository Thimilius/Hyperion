#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/vec2.hpp"

namespace Hyperion {

    class Application;

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
        
        inline static Vec2 s_mouse_position;
        inline static float s_mouse_scroll;
    public:
        inline static bool GetKeyDown(KeyCode key_code) { return s_keys_down[(s32)key_code]; }
        inline static bool GetKey(KeyCode key_code) { return s_keys[(s32)key_code]; }
        inline static bool GetKeyUp(KeyCode key_code) { return s_keys_up[(s32)key_code]; }

        inline static bool GetMouseButtonDown(MouseButtonCode mouse_button_code) { return s_mouse_buttons_down[(s32)mouse_button_code]; }
        inline static bool GetMouseButton(MouseButtonCode mouse_button_code) { return s_mouse_buttons[(s32)mouse_button_code]; }
        inline static bool GetMouseButtonUp(MouseButtonCode mouse_button_code) { return s_mouse_buttons_up[(s32)mouse_button_code]; }

        inline static Vec2 GetMousePosition() { return s_mouse_position; }
        inline static float GetMouseScroll() { return s_mouse_scroll; }
    private:
        Input() = delete;
        ~Input() = delete;

        static void Update();
        static void Reset();

        friend class Application;
    };

}
