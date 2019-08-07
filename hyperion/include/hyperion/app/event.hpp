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
        virtual CString GetName() const = 0;
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

    class CWindowCloseEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::WindowClose; }
        CString GetName() const override { return "WindowClose"; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowClose; }
    };

    class CWindowResizeEvent : public CEvent {
    private:
        u32 m_width;
        u32 m_height;
    public:
        CWindowResizeEvent(u32 width, u32 height) : m_width(width), m_height(height) { }

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        EEventType GetType() const override { return EEventType::WindowResize; }
        CString GetName() const override { return "WindowResize"; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowResize; }
    };

    class CWindowFocusEvent : public CEvent {
    private:
        bool m_focus;
    public:
        CWindowFocusEvent(bool focus) : m_focus(focus) { }

        inline bool GetFocus() const { return m_focus; }

        EEventType GetType() const override { return EEventType::WindowFocus; }
        CString GetName() const override { return "WindowFocus"; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowFocus; }
    };

    class CWindowMovedEvent : public CEvent
    {
    public:
        EEventType GetType() const override { return EEventType::WindowMoved; }
        CString GetName() const override { return "WindowMoved"; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowMoved; }
    };

    class CKeyEvent : public CEvent {
    private:
        EKeyCode m_key_code;
        EKeyModifier m_key_modifier;
    public:
        CKeyEvent(EKeyCode key_code, EKeyModifier key_modifier) : m_key_code(key_code), m_key_modifier(key_modifier) { }

        inline EKeyCode GetKeyCode() const { return m_key_code; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Keyboard; }
    };

    class CKeyPressedEvent : public CKeyEvent {
    public:
        CKeyPressedEvent(EKeyCode key_code, EKeyModifier key_modifier) : CKeyEvent(key_code, key_modifier) { }

        EEventType GetType() const override { return EEventType::KeyPressed; }
        CString GetName() const override { return "KeyPressed"; }

        static EEventType GetStaticType() { return EEventType::KeyPressed; }
    };

    class CKeyReleasedEvent : public CKeyEvent {
    public:
        CKeyReleasedEvent(EKeyCode key_code, EKeyModifier key_modifier) : CKeyEvent(key_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::KeyReleased; }
        CString GetName() const override { return "KeyReleased"; }

        static EEventType GetStaticType() { return EEventType::KeyReleased; }
    };

    class CKeyTypedEvent : public CEvent {
    private:
        u32 m_character;
        EKeyModifier m_key_modifier;
    public:
        CKeyTypedEvent(u32 character, EKeyModifier key_modifier) : m_character(character), m_key_modifier(key_modifier){ }

        inline u32 GetCharacter() const { return m_character; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventType GetType() const override { return EEventType::KeyTyped; }
        CString GetName() const override { return "KeyTyped"; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Keyboard; }

        static EEventType GetStaticType() { return EEventType::KeyTyped; }
    };

    class CMouseButtonEvent : public CEvent {
    private:
        EMouseButtonCode m_mouse_button_code;
        EKeyModifier m_key_modifier;
    public:
        CMouseButtonEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : m_mouse_button_code(mouse_button_code), m_key_modifier(key_modifier) { }

        inline EMouseButtonCode GetMouseButtonCode() const { return m_mouse_button_code; }
        inline bool HasKeyModifier(EKeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse & EEventCategory::MouseButton; }
    };

    class CMouseButtonPressedEvent : public CMouseButtonEvent {
    public:
        CMouseButtonPressedEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : CMouseButtonEvent(mouse_button_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::MouseButtonPressed; }
        CString GetName() const override { return "MouseButtonPressed"; }

        static EEventType GetStaticType() { return EEventType::MouseButtonPressed; }
    };

    class CMouseButtonReleasedEvent : public CMouseButtonEvent {
    public:
        CMouseButtonReleasedEvent(EMouseButtonCode mouse_button_code, EKeyModifier key_modifier) : CMouseButtonEvent(mouse_button_code, key_modifier) {}

        EEventType GetType() const override { return EEventType::MouseButtonReleased; }
        CString GetName() const override { return "MouseButtonReleased"; }

        static EEventType GetStaticType() { return EEventType::MouseButtonReleased; }
    };

    class CMouseMovedEvent : public CEvent {
    private:
        float m_x;
        float m_y;
    public:
        CMouseMovedEvent(float x, float y) : m_x(x), m_y(y) { }

        inline float GetX() const { return m_x; }
        inline float GetY() const { return m_y; }

        EEventType GetType() const override { return EEventType::MouseMoved; }
        CString GetName() const override { return "MouseMoved"; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse; }

        static EEventType GetStaticType() { return EEventType::MouseMoved; }
    };

    class CMouseScrolledEvent : public CEvent {
    private:
        float m_scroll;
    public:
        CMouseScrolledEvent(float scroll) : m_scroll(scroll) {}

        inline float GetScroll() const { return m_scroll; }

        EEventType GetType() const override { return EEventType::MouseScrolled; }
        CString GetName() const override { return "MouseScrolled"; }
        EEventCategory GetCategory() const override { return EEventCategory::Input & EEventCategory::Mouse; }

        static EEventType GetStaticType() { return EEventType::MouseScrolled; }
    };
}