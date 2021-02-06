#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {

    class WindowCloseEvent : public Event {
    public:
        EventType GetType() const override { return EventType::WindowClose; }
        EventCategory GetCategory() const override { return EventCategory::Window; }
    public:
        static EventType GetStaticType() { return EventType::WindowClose; }
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(uint32 width, uint32 height) : m_width(width), m_height(height) { }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }

        EventType GetType() const override { return EventType::WindowResize; }
        EventCategory GetCategory() const override { return EventCategory::Window; }
    public:
        static EventType GetStaticType() { return EventType::WindowResize; }
    private:
        uint32 m_width;
        uint32 m_height;
    };

    class WindowFocusEvent : public Event {
    public:
        WindowFocusEvent(bool focus) : m_focus(focus) { }

        inline bool GetFocus() const { return m_focus; }

        EventType GetType() const override { return EventType::WindowFocus; }
        EventCategory GetCategory() const override { return EventCategory::Window; }
    public:
        static EventType GetStaticType() { return EventType::WindowFocus; }
    private:
        bool m_focus;
    };

    class WindowMovedEvent : public Event {
    public:
        EventType GetType() const override { return EventType::WindowMoved; }
        EventCategory GetCategory() const override { return EventCategory::Window; }
    public:
        static EventType GetStaticType() { return EventType::WindowMoved; }
    };

    class WindowStateChangedEvent : public Event {
    public:
        WindowStateChangedEvent(WindowState state) : m_state(state) { }

        inline WindowState GetState() const { return m_state; }

        EventType GetType() const override { return EventType::WindowStateChanged; }
        EventCategory GetCategory() const override { return EventCategory::Window; }
    public:
        static EventType GetStaticType() { return EventType::WindowStateChanged; }
    private:
        WindowState m_state;
    };

}