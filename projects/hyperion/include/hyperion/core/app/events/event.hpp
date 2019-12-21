#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/app/input.hpp"

#include <functional>

namespace Hyperion {

    enum class EventType {
        None,

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
    };

    enum class EventCategory {
        None        = 0,
        App         = BIT(0),
        Window      = BIT(1),
        Input       = BIT(2),
        Keyboard    = BIT(3),
        Mouse       = BIT(4),
        MouseButton = BIT(5)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(EventCategory)

    class Event {
    private:
        bool m_handled = false;
    public:
        inline bool IsHandled() const { return m_handled; }
        inline void Handle() { m_handled = false; }

        virtual EventType GetType() const = 0;
        virtual EventCategory GetCategory() const = 0;

        inline bool IsInCategory(EventCategory category) const { return (GetCategory() & category) == category; }
    };

    class CEventDispatcher {
        template<typename T>
        using EventFunction = std::function<void(T &)>;
    private:
        Event &m_event;
    public:
        CEventDispatcher(Event &event)
            : m_event(event) {
        }

        template<typename T>
        bool Dispatch(EventFunction<T> func) {
            if (m_event.GetType() == T::GetStaticType()) {
                func(*(T *)& m_event);
                return true;
            }
            return false;
        }
    };
    
}