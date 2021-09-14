#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/graphics/graphics_backend.hpp"
#include "hyperion/platform/windows/windows_input.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;

    namespace Graphics {
        class GraphicsContext;
    }
    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsWindow : public INonCopyable {
    public:
        WindowsWindow(const WindowSettings &settings);
        ~WindowsWindow();
    public:
        void *GetNativePointer() const { return m_window_handle; }

        inline const String &GetTitle() const { return m_title; }
        void SetTitle(const String &title);

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        void SetSize(uint32 width, uint32 height);

        inline uint32 GetMinWidth() const { return m_min_width; }
        inline uint32 GetMinHeight() const { return m_min_height; }
        void SetMinimumSize(uint32 min_width, uint32 min_height);

        inline WindowMode GetWindowMode() const { return m_window_mode; }
        void SetWindowMode(WindowMode window_mode);
        inline WindowState GetWindowState() const { return m_window_state; }
        void SetWindowState(WindowState window_state);

        inline bool8 IsCursorVisible() const { return m_cursor_is_visible; }
        void SetCursorVisible(bool8 visible);
        inline CursorMode GetCursorMode() const { return m_cursor_mode; }
        void SetCursorMode(CursorMode mode);

        void SetIcon(const String &path);
    private:
        Graphics::GraphicsContext *CreateGraphicsContext(Graphics::GraphicsBackend graphics_backend);

        void Poll();
        void Show();
        
        InputImplementation *GetInput() const { return m_input; }
        void SetAppEventCallback(const AppEventCallbackFunction &app_event_callback);

        void SetupWindow(const WindowSettings &settings);
        Vector2 GetActualWindowSize(uint32 client_width, uint32 client_height) const;
        void DispatchAppEvent(AppEvent &app_event) const;
        void DispatchKeyAppEvent(KeyCode key_code, bool8 is_down) const;

        KeyCode TranslateKeyCode(uint32 w_param, uint32 l_param, bool8 is_down) const;
        MouseButtonCode TranslateMouseButtonCode(uint32 code) const;
        KeyModifier GetKeyModifier() const;
        uint32 GetMouseButtonFromMessage(uint32 message, uint32 w_param) const;
    private:
        static LRESULT CALLBACK MessageCallback(HWND window_handle, uint32 message, WPARAM first_message_param, LPARAM second_message_param);
    private:
        String m_title;

        uint32 m_width;
        uint32 m_height;

        uint32 m_min_width;
        uint32 m_min_height;

        WindowMode m_window_mode;
        WindowState m_window_state;

        bool8 m_is_focused;

        CursorMode m_cursor_mode;
        bool8 m_cursor_is_visible;

        AppEventCallbackFunction m_app_event_callback;

        WindowsInput *m_input;

        HWND m_window_handle;
        UINT_PTR m_timer;
        
        // We store the last window placement in order to switch correctly between windowed and borderless mode.
        WINDOWPLACEMENT m_previous_placement;

        // To distinguish the two shift keys we explicily store their previous state.
        // That way we can send out the correct key released events when appropriate.
        mutable bool8 m_left_shift_last_down = false;
        mutable bool8 m_right_shift_last_down = false;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderEngine;
    };

}