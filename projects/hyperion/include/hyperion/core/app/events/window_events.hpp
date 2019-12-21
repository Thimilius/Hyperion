#pragma once

#include "hyperion/core/app/events/event.hpp"

#include "hyperion/core/app/window.hpp"

namespace Hyperion {

    class WindowCloseEvent : public Event {
    public:
        EventType GetType() const override { return EventType::WindowClose; }
        EventCategory GetCategory() const override { return EventCategory::Window; }

        static EventType GetStaticType() { return EventType::WindowClose; }
    };

    class WindowResizeEvent : public Event {
    private:
        u32 m_width;
        u32 m_height;
    public:
        WindowResizeEvent(u32 width, u32 height) : m_width(width), m_height(height) {}

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        EventType GetType() const override { return EventType::WindowResize; }
        EventCategory GetCategory() const override { return EventCategory::Window; }

        static EventType GetStaticType() { return EventType::WindowResize; }
    };

    class WindowFocusEvent : public Event {
    private:
        bool m_focus;
    public:
        WindowFocusEvent(bool focus) : m_focus(focus) {}

        inline bool GetFocus() const { return m_focus; }

        EventType GetType() const override { return EventType::WindowFocus; }
        EventCategory GetCategory() const override { return EventCategory::Window; }

        static EventType GetStaticType() { return EventType::WindowFocus; }
    };

    class WindowMovedEvent : public Event {
    public:
        EventType GetType() const override { return EventType::WindowMoved; }
        EventCategory GetCategory() const override { return EventCategory::Window; }

        static EventType GetStaticType() { return EventType::WindowMoved; }
    };

    class WindowStateChangedEvent : public Event {
    private:
        WindowState m_state;
    public:
        WindowStateChangedEvent(WindowState state) : m_state(state) {}

        inline WindowState GetState() const { return m_state; }

        EventType GetType() const override { return EventType::WindowStateChanged; }
        EventCategory GetCategory() const override { return EventCategory::Window; }

        static EventType GetStaticType() { return EventType::WindowStateChanged; }
    };

}