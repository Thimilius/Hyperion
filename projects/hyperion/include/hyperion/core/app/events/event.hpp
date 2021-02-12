#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class EventType {
        AppDisplayChange,

        WindowClose,
        WindowResize,
        WindowFocus,
        WindowMoved,
        WindowStateChanged,

        KeyPressed,
        KeyReleased,
        KeyTyped,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,

        GamepadButtonPressed,
        GamepadButtonReleased,
        GamepadConnectionChanged,
    };

    enum class EventCategory {
        App         = BIT(0),
        Window      = BIT(1),
        Input       = BIT(2),
        Keyboard    = BIT(3),
        Mouse       = BIT(4),
        MouseButton = BIT(5),
        Gamepad     = BIT(6)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(EventCategory)

    class Event {
    public:
        inline bool IsHandled() const { return m_handled; }
        inline void Handle() { m_handled = false; }

        virtual EventType GetType() const = 0;
        virtual EventCategory GetCategory() const = 0;

        inline bool IsInCategory(EventCategory category) const { return (GetCategory() & category) == category; }
    private:
        bool m_handled = false;
    };

    using EventCallbackFunction = std::function<void(Event &)>;

    class EventDispatcher {
        template<typename T>
        using EventFunction = std::function<void(T &)>;
    public:
        EventDispatcher(Event &event) : m_event(event) { }
    public:
        template<typename T>
        bool Dispatch(EventFunction<T> func) {
            if (m_event.GetType() == T::GetStaticType()) {
                func(*(T *)& m_event);
                return true;
            }
            return false;
        }
    private:
        Event &m_event;
    };
    
}