//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
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
    None = 0,
    Shift = BIT(0),
    Control = BIT(1),
    Alt = BIT(2),
    Super = BIT(3),
    Capslock = BIT(4),
    Numlock = BIT(5)
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
    None,

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

    LeftShoulder,
    RightShoulder,

    LeftThumb,
    RightThumb,

    Last
  };

  enum class GamepadAxis {
    LeftStick,
    RightStick,

    LeftTrigger,
    RightTrigger,

    Last
  };

  enum class CursorMode {
    Default,
    Confined
  };

}