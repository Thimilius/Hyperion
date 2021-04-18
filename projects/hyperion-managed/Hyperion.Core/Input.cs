using System.Runtime.CompilerServices;

namespace Hyperion {

    public enum KeyCode {
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

        AltGr,
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

    public enum MouseButtonCode {
        None,

        Left,
        Right,
        Middle,

        Button4,
        Button5,

        Last
    };

    public static class Input {
        public static bool IsKeyDown(KeyCode keyCode) => Binding_IsKeyDown(keyCode);
        public static bool IsKeyHold(KeyCode keyCode) => Binding_IsKeyHold(keyCode);
        public static bool IsKeyUp(KeyCode keyCode) => Binding_IsKeyUp(keyCode);

        public static bool IsMouseButtonDown(MouseButtonCode mouseButtonCode) => Binding_IsMouseButtonDown(mouseButtonCode);
        public static bool IsMouseButtonHold(MouseButtonCode mouseButtonCode) => Binding_IsMouseButtonHold(mouseButtonCode);
        public static bool IsMouseButtonUp(MouseButtonCode mouseButtonCode) => Binding_IsMouseButtonUp(mouseButtonCode);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsKeyDown(KeyCode keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsKeyHold(KeyCode keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsKeyUp(KeyCode keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsMouseButtonDown(MouseButtonCode mouseButtonCode);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsMouseButtonHold(MouseButtonCode mouseButtonCode);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsMouseButtonUp(MouseButtonCode mouseButtonCode);
    }
}
