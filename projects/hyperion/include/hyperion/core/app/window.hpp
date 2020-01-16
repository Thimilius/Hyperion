#pragma once

#include "hyperion/common.hpp"

#include "hyperion/core/app/events/event.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/rendering/render_backend.hpp"

namespace Hyperion {

    namespace Rendering {
        class GraphicsContext;
    }

    class Window {
        using EventCallbackFunction = std::function<void(Event &)>;
    protected:
        String m_title;

        u32 m_width;
        u32 m_height;

        u32 m_min_width;
        u32 m_min_height;

        WindowMode m_window_mode;
        WindowState m_window_state;
        VSyncMode m_vsync_mode;

        EventCallbackFunction m_event_callback;

        Scope<Rendering::GraphicsContext> m_graphics_context;
    public:
        virtual ~Window() = default;

        virtual void *GetNativePointer() const = 0;

        inline const String &GetTitle() const { return m_title; }
        virtual void SetTitle(const String &title) = 0;

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        virtual void SetSize(u32 width, u32 height) = 0;

        inline u32 GetMinWidth() const { return m_min_width; }
        inline u32 GetMinHeight() const { return m_min_height; }
        virtual void SetMinimumSize(u32 min_width, u32 min_height) = 0;

        inline WindowMode GetWindowMode() const { return m_window_mode; }
        virtual void SetWindowMode(WindowMode window_mode) = 0;

        inline WindowState GetWindowState() const { return m_window_state; }
        virtual void SetWindowState(WindowState window_state) = 0;

        inline VSyncMode GetVSyncMode() const { return m_vsync_mode; }
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
        
        virtual void SetIcon(const String &path) = 0;
    protected:
        virtual void Update() = 0;
        virtual void Show() = 0;

        virtual InputImplementation *GetInput() const = 0;
        virtual void SetEventCallbackFunction(const EventCallbackFunction &event_callback) { m_event_callback = event_callback; }

        static Ref<Window> Create(const WindowSettings &settings, Rendering::RenderBackend render_backend);

        friend class Application;
    };

}