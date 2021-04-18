#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/platform/windows/windows_input.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowSettings &settings);
        ~WindowsWindow();
    public:
        void *GetNativePointer() const override { return m_window_handle; }

        void SetTitle(const String &title) override;
        void SetSize(uint32 width, uint32 height) override;
        void SetMinimumSize(uint32 min_width, uint32 min_height) override;
        void SetWindowMode(WindowMode window_mode) override;
        void SetWindowState(WindowState window_state) override;

        void SetCursorVisible(bool visible) override;
        void SetCursorMode(CursorMode mode) override;

        void SetIcon(const String &path) override;
    private:
        Graphics::IGraphicsContext *CreateGraphicsContext(Rendering::RenderBackend render_backend) override;

        void Poll() override;
        void Show() override;
        
        InputImplementation *GetInput() const override { return m_input; }
        void SetEventCallback(const EventCallbackFunction &event_callback) override;

        void SetupWindow(const WindowSettings &settings);
        Vec2 GetActualWindowSize(uint32 client_width, uint32 client_height) const;
        void DispatchEvent(Event &event) const;
        void DispatchKeyEvent(KeyCode key_code, bool is_down) const;

        KeyCode TranslateKeyCode(uint32 w_param, uint32 l_param, bool is_down) const;
        MouseButtonCode TranslateMouseButtonCode(uint32 code) const;
        KeyModifier GetKeyModifier() const;
        uint32 GetMouseButtonFromMessage(uint32 message, uint32 w_param) const;
    private:
        static LRESULT CALLBACK MessageCallback(HWND window_handle, uint32 message, WPARAM first_message_param, LPARAM second_message_param);
    private:
        WindowsInput *m_input;

        HWND m_window_handle;
        UINT_PTR m_timer;
        
        // We store the last window placement in order to switch correctly between windowed and borderless mode.
        WINDOWPLACEMENT m_previous_placement;

        // To distinguish the two shift keys we explicily store their previous state.
        // That way we can send out the correct key released events when appropriate.
        mutable bool m_left_shift_last_down = false;
        mutable bool m_right_shift_last_down = false;
    };

}