#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/platform/windows/windows_input.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsWindow : public Window {
        using EventCallbackFunction = std::function<void(Event &)>;
    public:
        WindowsWindow(const WindowSettings &settings, Rendering::RenderBackend render_backend);
        ~WindowsWindow();

        void *GetNativePointer() const override { return m_window_handle; }

        void SetTitle(const String &title) override;
        void SetSize(u32 width, u32 height) override;
        void SetMinimumSize(u32 min_width, u32 min_height) override;
        void SetWindowMode(WindowMode window_mode) override;
        void SetWindowState(WindowState window_state) override;
        void SetVSyncMode(VSyncMode vsync_mode) override;

        void SetIcon(const String &path) override;
    private:
        void Update() override;
        void Show() override;
        
        void SetupWindow(const WindowSettings &settings);

        InputImplementation *GetInput() const override { return m_input; }
        void SetEventCallbackFunction(const EventCallbackFunction &event_callback) override;

        Vec2 GetActualWindowSize(u32 client_width, u32 client_height);

        void CreateContext(Rendering::RenderBackend backend_api);
        void DispatchEvent(Event &event) const;

        KeyCode TranslateKeyCode(u32 w_param, u32 l_param) const;
        MouseButtonCode TranslateMouseButtonCode(u32 code) const;
        KeyModifier GetKeyModifier() const;
        u32 GetMouseButtonFromMessage(u32 message, u32 w_param) const;

        static LRESULT CALLBACK MessageCallback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);
    private:
        WindowsInput *m_input;

        HWND m_window_handle;
        
        // We store the last window placement in order to switch correctly
        // between windowed and borderless mode
        WINDOWPLACEMENT m_previous_placement;
        
        // To distinguish the two shift keys we explicily store their previous state
        // so that we can send out the correct key released events when appropriate
        mutable bool m_left_shift_last_down = false;
        mutable bool m_right_shift_last_down = false;
    };

}