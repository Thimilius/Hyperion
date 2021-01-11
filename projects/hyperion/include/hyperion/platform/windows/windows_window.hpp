#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/platform/windows/windows_input.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowSettings &settings);
        ~WindowsWindow();

        void *GetNativePointer() const override { return m_window_handle; }

        void SetTitle(const String &title) override;
        void SetSize(u32 width, u32 height) override;
        void SetMinimumSize(u32 min_width, u32 min_height) override;
        void SetWindowMode(WindowMode window_mode) override;
        void SetWindowState(WindowState window_state) override;

        void SetCursorVisible(bool visible) override;
        void SetCursorMode(CursorMode mode) override;

        void SetIcon(const String &path) override;
    private:
        Rendering::GraphicsContext *CreateGraphicsContext(Rendering::RenderBackend render_backend) override;

        void Poll() override;
        void Show() override;
        
        InputImplementation *GetInput() const override { return m_input; }
        void SetEventCallback(const EventCallbackFunction &event_callback) override;

        void SetupWindow(const WindowSettings &settings);
        Vec2 GetActualWindowSize(u32 client_width, u32 client_height) const;
        void DispatchEvent(Event &event) const;

        KeyCode TranslateKeyCode(u32 w_param, u32 l_param) const;
        MouseButtonCode TranslateMouseButtonCode(u32 code) const;
        KeyModifier GetKeyModifier() const;
        u32 GetMouseButtonFromMessage(u32 message, u32 w_param) const;

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    private:
        WindowsInput *m_input;

        HWND m_window_handle;
        UINT_PTR m_timer;
        
        // We store the last window placement in order to switch correctly
        // between windowed and borderless mode
        WINDOWPLACEMENT m_previous_placement;

        // To distinguish the two shift keys we explicily store their previous state
        // so that we can send out the correct key released events when appropriate
        mutable bool m_left_shift_last_down = false;
        mutable bool m_right_shift_last_down = false;
    };

}