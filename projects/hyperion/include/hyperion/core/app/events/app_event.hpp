#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AppEventType {
        DisplayChange,

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

    enum class AppEventCategory {
        Display     = BIT(0),
        Window      = BIT(1),
        Input       = BIT(2),
        Keyboard    = BIT(3),
        Mouse       = BIT(4),
        MouseButton = BIT(5),
        Gamepad     = BIT(6)
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(AppEventCategory)

    class AppEvent {
    public:
        inline bool8 IsHandled() const { return m_handled; }
        inline void Handle() { m_handled = false; }

        virtual AppEventType GetType() const = 0;
        virtual AppEventCategory GetCategory() const = 0;

        inline bool8 IsInCategory(AppEventCategory category) const { return (GetCategory() & category) == category; }
    private:
        bool8 m_handled = false;
    };

    using AppEventCallbackFunction = std::function<void(AppEvent &)>;

    class AppEventDispatcher {
        template<typename T>
        using AppEventFunction = std::function<void(T &)>;
    public:
        AppEventDispatcher(AppEvent &app_event) : m_app_event(app_event) { }
    public:
        template<typename T>
        bool8 Dispatch(AppEventFunction<T> func) {
            if (m_app_event.GetType() == T::GetStaticType()) {
                func(*static_cast<T *>(&m_app_event));
                return true;
            }
            return false;
        }
    private:
        AppEvent &m_app_event;
    };
    
}