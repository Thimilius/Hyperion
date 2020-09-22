#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {
    class WindowsWindow;
}

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

    class InputImplementation {
        using EventCallbackFunction = std::function<void(Event &)>;
    public:
        virtual ~InputImplementation() = default;

        virtual bool GetKeyDown(KeyCode key_code) const = 0;
        virtual bool GetKey(KeyCode key_code) const = 0;
        virtual bool GetKeyUp(KeyCode key_code) const = 0;

        virtual Vec2 GetMousePosition() const = 0;
        virtual f32 GetMouseScroll() const = 0;

        virtual bool GetMouseButtonDown(MouseButtonCode mouse_button_code) const = 0;
        virtual bool GetMouseButton(MouseButtonCode mouse_button_code) const = 0;
        virtual bool GetMouseButtonUp(MouseButtonCode mouse_button_code) const = 0;

        virtual const Vector<Gamepad> &GetConnectedGamepads() const = 0;
        virtual Vec2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const = 0;
        virtual void SetGamepadVibration(Gamepad gamepad, f32 left_vibration, f32 right_vibration) = 0;

        virtual f32 GetGamepadDeadZone() const = 0;
        virtual void SetGamepadDeadZone(f32 gamepad_dead_zone) = 0;

        virtual bool GetGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;
        virtual bool GetGamepadButton(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;
        virtual bool GetGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;
    protected:
        inline void DispatchEvent(Event &event) const {
            if (m_event_callback) {
                m_event_callback(event);
            }
        }

        void SetEventCallbackFunction(const EventCallbackFunction &event_callback) { m_event_callback = event_callback; }
    private:
        EventCallbackFunction m_event_callback;
    };

    class Input {
    public:
        inline static bool GetKeyDown(KeyCode key_code) { return s_input_implementation->GetKeyDown(key_code); }
        inline static bool GetKey(KeyCode key_code) { return s_input_implementation->GetKey(key_code); }
        inline static bool GetKeyUp(KeyCode key_code) { return s_input_implementation->GetKeyUp(key_code); }

        inline static Vec2 GetMousePosition() { return s_input_implementation->GetMousePosition(); }
        inline static f32 GetMouseScroll() { return s_input_implementation->GetMouseScroll(); }

        inline static bool GetMouseButtonDown(MouseButtonCode mouse_button_code) { return s_input_implementation->GetMouseButtonDown(mouse_button_code); }
        inline static bool GetMouseButton(MouseButtonCode mouse_button_code) { return s_input_implementation->GetMouseButton(mouse_button_code); }
        inline static bool GetMouseButtonUp(MouseButtonCode mouse_button_code) { return s_input_implementation->GetMouseButtonUp(mouse_button_code); }

        inline static const Vector<Gamepad> &GetConnectedGamepads() { return s_input_implementation->GetConnectedGamepads(); }
        inline static Vec2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) { return s_input_implementation->GetGamepadAxis(gamepad, axis); }
        inline static void SetGamepadVibration(Gamepad gamepad, f32 left_vibration, f32 right_vibration) { s_input_implementation->SetGamepadVibration(gamepad, left_vibration, right_vibration); }

        inline static f32 GetGamepadDeadZone() { return s_input_implementation->GetGamepadDeadZone(); }
        inline static void GetGamepadDeadZone(f32 gamepad_dead_zone) { s_input_implementation->SetGamepadDeadZone(gamepad_dead_zone); }

        inline static bool GetGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_input_implementation->GetGamepadButtonDown(gamepad, gamepad_button_code); }
        inline static bool GetGamepadButton(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_input_implementation->GetGamepadButton(gamepad, gamepad_button_code); }
        inline static bool GetGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) { return s_input_implementation->GetGamepadButtonUp(gamepad, gamepad_button_code); }
    private:
        Input() = delete;
        ~Input() = delete;
    private:
        static Scope<InputImplementation> s_input_implementation;
    private:
        friend class Hyperion::WindowsWindow;
    };

}
