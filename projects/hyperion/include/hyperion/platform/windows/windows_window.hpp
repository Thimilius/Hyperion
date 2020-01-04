#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/core/math/vec2.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsWindow : public Window {
    private:
        HWND m_window_handle;
        WINDOWPLACEMENT m_previous_placement;
    public:
        WindowsWindow(const WindowSettings &settings, Rendering::RenderBackend render_backend);
        ~WindowsWindow();

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

        void CreateContext(Rendering::RenderBackend backend_api);
        void DispatchEvent(Event &event) const;

        KeyCode TranslateKeyCode(u32 w_param, u32 l_param) const;
        MouseButtonCode TranslateMouseButtonCode(u32 code) const;
        KeyModifier GetKeyModifier() const;
        u32 GetMouseButtonFromMessage(u32 message, u32 w_param) const;

        void CheckForConnectedGamepads(bool send_events);
        Gamepad GetGamepadFromId(u32 id);

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    };

}