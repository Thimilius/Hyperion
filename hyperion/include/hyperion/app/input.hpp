#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/vec2.hpp"

namespace Hyperion {

    class CApplication;

    enum class EKeyCode {
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

        Left,
        Right,
        Middle,

        Button4,
        Button5,

        Last
    };

    enum class EKeyModifier {
        None     = 0,
        Shift    = 1 << 0,
        Control  = 1 << 1,
        Alt      = 1 << 2,
        Super    = 1 << 3,
        Capslock = 1 << 4,
        Numlock  = 1 << 5
    };
    inline EKeyModifier operator|(EKeyModifier a, EKeyModifier b) {
        return static_cast<EKeyModifier>(static_cast<s32>(a) | static_cast<s32>(b));
    }
    inline EKeyModifier operator|=(EKeyModifier &a, EKeyModifier b) {
        return a = a | b;
    }
    inline EKeyModifier operator&(EKeyModifier a, EKeyModifier b) {
        return static_cast<EKeyModifier>(static_cast<s32>(a) & static_cast<s32>(b));
    }

    class CInput {
    private:
        static bool s_keys_down[(s32)EKeyCode::Last];
        static bool s_keys[(s32)EKeyCode::Last];
        static bool s_keys_last[(s32)EKeyCode::Last];
        static bool s_keys_up[(s32)EKeyCode::Last];

        static bool s_mouse_buttons_down[(s32)EMouseButtonCode::Last];
        static bool s_mouse_buttons[(s32)EMouseButtonCode::Last];
        static bool s_mouse_buttons_last[(s32)EMouseButtonCode::Last];
        static bool s_mouse_buttons_up[(s32)EMouseButtonCode::Last];

        static Math::SVec2 s_mouse_position;
        static float s_mouse_scroll;
    public:
        inline static bool GetKeyDown(EKeyCode key_code) { return s_keys_down[(s32)key_code]; }
        inline static bool GetKey(EKeyCode key_code) { return s_keys[(s32)key_code]; }
        inline static bool GetKeyUp(EKeyCode key_code) { return s_keys_up[(s32)key_code]; }

        inline static bool GetMouseButtonDown(EMouseButtonCode mouse_button_code) { return s_mouse_buttons_down[(s32)mouse_button_code]; }
        inline static bool GetMouseButton(EMouseButtonCode mouse_button_code) { return s_mouse_buttons[(s32)mouse_button_code]; }
        inline static bool GetMouseButtonUp(EMouseButtonCode mouse_button_code) { return s_mouse_buttons_up[(s32)mouse_button_code]; }

        inline static Math::SVec2 GetMousePosition() { return s_mouse_position; }
        inline static float GetMouseScroll() { return s_mouse_scroll; }
    private:
        CInput() = delete;
        ~CInput() = delete;

        static void Update();

        friend class CApplication;
    };

}
