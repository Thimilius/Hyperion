#pragma once

#include "hyperion/common.hpp"

#include "hyperion/core/app/events/event.hpp"

namespace Hyperion {

    class Application;
    namespace Rendering {
        class GraphicsContext;
    }

    enum class WindowMode {
        Windowed,
        Borderless
    };

    enum class WindowState {
        Normal,
        Minimized,
        Maximized
    };

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    class Window {
        using EventCallbackFunction = std::function<void(Event &)>;
    protected:
        String m_title;

        u32 m_width;
        u32 m_height;

        WindowMode m_window_mode;
        WindowState m_window_state;
        VSyncMode m_vsync_mode;

        EventCallbackFunction m_event_callback;

        Scope<Rendering::GraphicsContext> m_graphics_context;
    public:

        virtual ~Window() = default;

        inline const String &GetTitle() const { return m_title; }
        virtual void SetTitle(const String &title) = 0;

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        virtual void SetSize(u32 width, u32 height) = 0;

        inline WindowMode GetWindowMode() const { return m_window_mode; }
        virtual void SetWindowMode(WindowMode window_mode) = 0;

        inline WindowState GetWindowState() const { return m_window_state; }
        virtual void SetWindowState(WindowState window_state) = 0;

        inline VSyncMode GetVSyncMode() const { return m_vsync_mode; }
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;

        inline const Scope<Rendering::GraphicsContext> &GetGraphicsContext() const { return m_graphics_context; }
        
        virtual void SetIcon(const String &path) = 0;
    protected:
        virtual void Update() const = 0;
        virtual void Show() const = 0;

        void SetEventCallbackFunction(const EventCallbackFunction &callback) { m_event_callback = callback; }

        static Window *Create(const String &title, u32 width, u32 height, WindowMode window_mode, VSyncMode vsync_mode);

        friend class Application;
    };

}