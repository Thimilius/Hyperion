#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/core/app/events/event.hpp"
#include "hyperion/rendering/render_backend.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;

    namespace Graphics {
        class IGraphicsContext;
    }
    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class CursorMode {
        Default,
        Confined
    };

    class Window {
    public:
        virtual ~Window() = default;
    public:
        virtual void *GetNativePointer() const = 0;

        inline const String &GetTitle() const { return m_title; }
        virtual void SetTitle(const String &title) = 0;

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        virtual void SetSize(uint32 width, uint32 height) = 0;

        inline uint32 GetMinWidth() const { return m_min_width; }
        inline uint32 GetMinHeight() const { return m_min_height; }
        virtual void SetMinimumSize(uint32 min_width, uint32 min_height) = 0;

        inline WindowMode GetWindowMode() const { return m_window_mode; }
        virtual void SetWindowMode(WindowMode window_mode) = 0;
        inline WindowState GetWindowState() const { return m_window_state; }
        virtual void SetWindowState(WindowState window_state) = 0;
        
        inline bool IsCursorVisible() const { return m_cursor_is_visible; }
        virtual void SetCursorVisible(bool visible) = 0;
        inline CursorMode GetCursorMode() const { return m_cursor_mode; }
        virtual void SetCursorMode(CursorMode mode) = 0;

        virtual void SetIcon(const String &path) = 0;
    protected:
        virtual Graphics::IGraphicsContext *CreateGraphicsContext(Rendering::RenderBackend render_backend) = 0;

        virtual void Poll() = 0;
        virtual void Show() = 0;

        virtual InputImplementation *GetInput() const = 0;
        virtual void SetEventCallback(const EventCallbackFunction &event_callback) { m_event_callback = event_callback; }

        static Window *Create(const WindowSettings &settings);
    protected:
        String m_title;

        uint32 m_width;
        uint32 m_height;

        uint32 m_min_width;
        uint32 m_min_height;

        WindowMode m_window_mode;
        WindowState m_window_state;

        bool m_is_focused;

        CursorMode m_cursor_mode;
        bool m_cursor_is_visible;

        EventCallbackFunction m_event_callback;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderEngine;
    };

}
