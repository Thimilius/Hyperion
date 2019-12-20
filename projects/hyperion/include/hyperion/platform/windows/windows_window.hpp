#pragma once

#include "hyperion/app/window.hpp"
#include "hyperion/math/vec2.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsWindow : public Window {
    private:
        HWND m_window_handle;
        WINDOWPLACEMENT m_previous_placement;
    public:
        WindowsWindow(const String &title, u32 width, u32 height, WindowMode window_mode, VSyncMode vsync_mode);

        void SetTitle(const String &title) override;
        void SetSize(u32 width, u32 height) override;
        void SetWindowMode(WindowMode window_mode) override;
        void SetWindowState(WindowState window_state) override;
        void SetVSyncMode(VSyncMode vsync_mode) override;

        void SetIcon(const String &path) override;
    private:
        void Update() const override;
        void Show() const override;

        Vec2 GetActualWindowSize(u32 client_width, u32 client_height);

        KeyCode TranslateKeyCode(u32 w_param, u32 l_param) const;
        MouseButtonCode TranslateMouseButtonCode(u32 code) const;
        KeyModifier GetKeyModifier() const;

        void CreateContext();
        void DispatchEvent(Event &event) const;
        u32 GetMouseButtonFromMessage(u32 message, u32 w_param) const;

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    };

}