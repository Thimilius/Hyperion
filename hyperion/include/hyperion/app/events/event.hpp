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
        App         = 1 << 0,
        Window      = 1 << 1,
        Input       = 1 << 2,
        Keyboard    = 1 << 3,
        Mouse       = 1 << 4,
        MouseButton = 1 << 5
    };
    inline EEventCategory operator|(EEventCategory a, EEventCategory b) {
        return static_cast<EEventCategory>(static_cast<s32>(a) | static_cast<s32>(b));
    }
    inline EEventCategory operator|=(EEventCategory &a, EEventCategory b) {
        return a = a | b;
    }
    inline EEventCategory operator&(EEventCategory a, EEventCategory b) {
        return static_cast<EEventCategory>(static_cast<s32>(a) & static_cast<s32>(b));
    }

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