#pragma once

#include "hyperion/common.hpp"

#include "event.hpp"

namespace Hyperion {

    class CApplication;
    namespace Rendering {
        class CGraphicsContext;
    }

    enum class EWindowMode {
        Windowed,
        Borderless,
        Fullscreen
    };

    enum class EVSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    class CWindow {
        using EventCallbackFunction = std::function<void(CEvent &)>;
    protected:
        CString m_title;
        u32 m_width;
        u32 m_height;
        EWindowMode m_window_mode;
        EVSyncMode m_vsync_mode;

        EventCallbackFunction m_event_callback;

        Rendering::CGraphicsContext *m_graphics_context;
    public:

        virtual ~CWindow() = default;

        inline CString GetTitle() const { return m_title; }
        virtual void SetTitle(CString title) = 0;

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        virtual void SetSize(u32 width, u32 height) = 0;

        inline EWindowMode GetWindowMode() const { return m_window_mode; }
        virtual void SetWindowMode(EWindowMode window_mode) = 0;

        inline EVSyncMode GetVSyncMode() const { return m_vsync_mode; }
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;

        virtual void SetIcon(const char *path) = 0;

        static CWindow *Create(const CString &title, u32 width, u32 height, EWindowMode window_mode);
    protected:
        virtual void Update() const = 0;
        virtual void Show() const = 0;

        virtual EKeyCode GetKeyCode(u32 code) const = 0;
        virtual EMouseButtonCode GetMouseButtonCode(u32 code) const = 0;
        virtual EKeyModifier GetKeyModifier() const = 0;

        void SetEventCallbackFunction(const EventCallbackFunction &callback) { m_event_callback = callback; }

        friend class CApplication;
    };

}