#pragma once

#include "event.hpp"

namespace Hyperion::Events {

    class CWindowCloseEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::WindowClose; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowClose; }
    };

    class CWindowResizeEvent : public CEvent {
    private:
        u32 m_width;
        u32 m_height;
    public:
        CWindowResizeEvent(u32 width, u32 height) : m_width(width), m_height(height) {}

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        EEventType GetType() const override { return EEventType::WindowResize; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowResize; }
    };

    class CWindowFocusEvent : public CEvent {
    private:
        bool m_focus;
    public:
        CWindowFocusEvent(bool focus) : m_focus(focus) {}

        inline bool GetFocus() const { return m_focus; }

        EEventType GetType() const override { return EEventType::WindowFocus; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowFocus; }
    };

    class CWindowMovedEvent : public CEvent {
    public:
        EEventType GetType() const override { return EEventType::WindowMoved; }
        EEventCategory GetCategory() const override { return EEventCategory::Window; }

        static EEventType GetStaticType() { return EEventType::WindowMoved; }
    };

}