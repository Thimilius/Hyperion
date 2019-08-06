#pragma once

#include "hyperion/common.hpp"
#include "hyperion/app/input.hpp"

#include <functional>

namespace Hyperion {

    enum class EEventType {
        None,
        WindowClose, WindowResize, WindowFocus, WindowMoved,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum class EEventCategory {
        None        = 0,
        Window      = 1 << 0,
        Input       = 1 << 1,
        Keyboard    = 1 << 2,
        Mouse       = 1 << 3,
        MouseButton = 1 << 4
    };

    class CEvent {
    private:
        bool m_handled = false;
    public:
        inline bool GetHandled() const { return m_handled; }
        inline void SetHandled(bool handled) { m_handled = handled; }

        virtual EEventType GetType() const = 0;
        virtual CString GetName() const = 0;
        virtual u32 GetCategory() const = 0;

        inline bool IsInCategory(EEventCategory category) const { return GetCategory() & (u32)category; }
    };

    class CEventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T &)>;
    private:
        CEvent &m_event;
    public:
        CEventDispatcher(CEvent &event)
            : m_event(event) {
        }

        template<typename T>
        bool Dispatch(EventFn<T> func) {
            if (m_event.GetType() == T::GetStaticType()) {
                m_event.SetHandled(func(*(T *)& m_event));
                return true;
            }
            return false;
        }
    };

    class CWindowCloseEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::WindowClose; }
        CString GetName() const override { return "WindowClose"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Window; }

        EEventType GetStaticType() { return EEventType::WindowClose; }
    };

    class CWindowResizeEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::WindowResize; }
        CString GetName() const override { return "WindowResize"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Window; }

        EEventType GetStaticType() { return EEventType::WindowResize; }
    };

    class CWindowFocusEvent : public CEvent {
    private:
        bool m_focus;
    public:
        CWindowFocusEvent(bool focus) : m_focus(focus) { }

        inline bool GetFocus() const { return m_focus; }

        EEventType GetType() const override { return EEventType::WindowFocus; }
        CString GetName() const override { return "WindowFocus"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Window; }

        EEventType GetStaticType() { return EEventType::WindowFocus; }
    };

    class CWindowMovedEvent : public CEvent
    {
    public:
        EEventType GetType() const override { return EEventType::WindowMoved; }
        CString GetName() const override { return "WindowMoved"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Window; }

        EEventType GetStaticType() { return EEventType::WindowMoved; }
    };

    class CKeyEvent : public CEvent {
    protected:
        EKeyCode m_key_code;
    public:
        CKeyEvent(EKeyCode key_code) : m_key_code(key_code) { }

        inline EKeyCode GetKeyCode() const { return m_key_code; }

        u32 GetCategory() const override { return (u32)EEventCategory::Input & (u32)EEventCategory::Keyboard; }
    };

    class CKeyPressedEvent : public CKeyEvent {
    public:
        CKeyPressedEvent(EKeyCode key_code) : CKeyEvent(key_code) { }

        EEventType GetType() const override { return EEventType::KeyPressed; }
        CString GetName() const override { return "KeyPressed"; }

        EEventType GetStaticType() { return EEventType::KeyPressed; }
    };

    class CKeyReleasedEvent : public CKeyEvent {
    public:
        CKeyReleasedEvent(EKeyCode key_code) : CKeyEvent(key_code) {}

        EEventType GetType() const override { return EEventType::KeyReleased; }
        CString GetName() const override { return "KeyReleased"; }

        EEventType GetStaticType() { return EEventType::KeyReleased; }
    };

    class CKeyTypedEvent : public CKeyEvent {
    public:
        CKeyTypedEvent(EKeyCode key_code) : CKeyEvent(key_code) {}

        EEventType GetType() const override { return EEventType::KeyTyped; }
        CString GetName() const override { return "KeyTyped"; }

        EEventType GetStaticType() { return EEventType::KeyTyped; }
    };

    class CMouseButtonEvent : public CEvent {
    private:
        EMouseButtonCode m_mouse_button_code;
    public:
        CMouseButtonEvent(EMouseButtonCode mouse_button_code) : m_mouse_button_code(mouse_button_code) { }

        inline EMouseButtonCode GetMouseButtonCode() const { return m_mouse_button_code; }

        u32 GetCategory() const override { return (u32)EEventCategory::Input & (u32)EEventCategory::Mouse & (u32)EEventCategory::MouseButton; }
    };

    class CMouseButtonPressed : public CMouseButtonEvent {
    public:
        CMouseButtonPressed(EMouseButtonCode mouse_button_code) : CMouseButtonEvent(mouse_button_code) {}

        EEventType GetType() const override { return EEventType::MouseButtonPressed; }
        CString GetName() const override { return "MouseButtonPressed"; }

        EEventType GetStaticType() { return EEventType::MouseButtonPressed; }
    };

    class CMouseButtonReleased : public CMouseButtonEvent {
    public:
        CMouseButtonReleased(EMouseButtonCode mouse_button_code) : CMouseButtonEvent(mouse_button_code) {}

        EEventType GetType() const override { return EEventType::MouseButtonReleased; }
        CString GetName() const override { return "MouseButtonReleased"; }

        EEventType GetStaticType() { return EEventType::MouseButtonReleased; }
    };

    class CMouseMoved : public CEvent {
    private:
        float m_x;
        float m_y;
    public:
        CMouseMoved(float x, float y) : m_x(x), m_y(y) { }

        inline float GetX() const { return m_x; }
        inline float GetY() const { return m_y; }

        EEventType GetType() const override { return EEventType::MouseMoved; }
        CString GetName() const override { return "MouseMoved"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Input & (u32)EEventCategory::Mouse; }

        EEventType GetStaticType() { return EEventType::MouseMoved; }
    };

    class CMouseScrolled : public CEvent {
    private:
        float m_x_offset;
        float m_y_offset;
    public:
        CMouseScrolled(float x, float y) : m_x_offset(x), m_y_offset(y) {}

        inline float GetXOffset() const { return m_x_offset; }
        inline float GetYOffset() const { return m_y_offset; }

        EEventType GetType() const override { return EEventType::MouseScrolled; }
        CString GetName() const override { return "MouseScrolled"; }
        u32 GetCategory() const override { return (u32)EEventCategory::Input & (u32)EEventCategory::Mouse; }

        EEventType GetStaticType() { return EEventType::MouseScrolled; }
    };
}