#pragma once

#include "hyperion/common.hpp"
#include "hyperion/app/input.hpp"

#include <functional>

namespace Hyperion::Events {

    enum class EEventType {
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

    enum class EEventCategory {
        None        = 0,
        App         = BIT(0),
        Window      = BIT(1),
        Input       = BIT(2),
        Keyboard    = BIT(3),
        Mouse       = BIT(4),
        MouseButton = BIT(5)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(EEventCategory)

    class CEvent {
    private:
        bool m_handled = false;
    public:
        inline bool IsHandled() const { return m_handled; }
        inline void Handle() { m_handled = false; }

        virtual EEventType GetType() const = 0;
        virtual EEventCategory GetCategory() const = 0;

        inline bool IsInCategory(EEventCategory category) const { return (GetCategory() & category) == category; }
    };

    class CEventDispatcher {
        template<typename T>
        using EventFunction = std::function<void(T &)>;
    private:
        CEvent &m_event;
    public:
        CEventDispatcher(CEvent &event)
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