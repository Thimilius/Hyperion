#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CApplication;

    enum class EKeyCode {
        NONE,

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

        LeftWindows,
        RightWindows,
        Application,

        Control,
        LeftControl,
        RightControl,

        Shift,
        LeftShift,
        RightShift,

        Alt,
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

    enum class EMouseButtonCode {
        None,

        MouseLeft,
        MouseRight,
        MouseMiddle,

        Last
    };

    class CInput {
    private:
        static bool s_keys_down[(u32)EKeyCode::Last];
        static bool s_keys[(u32)EKeyCode::Last];
        static bool s_keys_last[(u32)EKeyCode::Last];
        static bool s_keys_up[(u32)EKeyCode::Last];

        static bool s_mouse_buttons_down[(u32)EMouseButtonCode::Last];
        static bool s_mouse_buttons[(u32)EMouseButtonCode::Last];
        static bool s_mouse_buttons_last[(u32)EMouseButtonCode::Last];
        static bool s_mouse_buttons_up[(u32)EMouseButtonCode::Last];
    public:
        inline static bool GetKeyDown(EKeyCode key_code) { return s_keys_down[(u32)key_code]; }
        inline static bool GetKey(EKeyCode key_code) { return s_keys[(u32)key_code]; }
        inline static bool GetKeyUp(EKeyCode key_code) { return s_keys_up[(u32)key_code]; }

        inline static bool GetMouseButtonDown(EMouseButtonCode mouse_button_code) { return s_mouse_buttons_down[(u32)mouse_button_code]; }
        inline static bool GetMouseButton(EMouseButtonCode mouse_button_code) { return s_mouse_buttons[(u32)mouse_button_code]; }
        inline static bool GetMouseButtonUp(EMouseButtonCode mouse_button_code) { return s_mouse_buttons_up[(u32)mouse_button_code]; }
    private:
        CInput() = delete;
        ~CInput() = delete;

        static void Update();

        friend class CApplication;
    };

}
