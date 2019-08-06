#include "hyppch.hpp"

#include "windows_window.hpp"

namespace Hyperion {

    static LRESULT CALLBACK window_callback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param);

    CWindow *CWindow::Create(CString title, Math::SVec2 size, EWindowMode window_mode, EVSyncMode vsync_mode) {
        return new CWindowsWindow(title, size, window_mode, vsync_mode);
    }

    CWindowsWindow::CWindowsWindow(CString title, Math::SVec2 size, EWindowMode window_mode, EVSyncMode vsync_mode) {
        m_title = title;
        m_size = size;
        m_window_mode = window_mode;
        m_vsync_mode = vsync_mode;

        u32 window_styles = WS_OVERLAPPEDWINDOW; //& ~WS_THICKFRAME & ~WS_MAXIMIZEBOX
        const char *window_class_name = "HYPERION_WINDOW_CLASS";
        HINSTANCE instance = GetModuleHandleA(NULL);

        WNDCLASSEXA window_class = { 0 };
        window_class.cbSize = sizeof(window_class);
        window_class.lpszClassName = window_class_name;
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.hInstance = instance;
        window_class.lpfnWndProc = window_callback;
        window_class.hCursor = LoadCursorA(NULL, (LPSTR)IDC_ARROW);

        RegisterClassExA(&window_class);

        RECT window_rect = {0};
        window_rect.right = (LONG)size.x;
        window_rect.bottom = (LONG)size.y;
        AdjustWindowRect(&window_rect, window_styles, false);

        m_window_handle = CreateWindowExA(
            0,
            window_class_name,
            title.ToCString(),
            window_styles,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            NULL,
            NULL,
            instance,
            NULL
        );

        if (m_window_handle == NULL) {
            HYP_CORE_ERROR("Failed to create window!");
        }

        m_device_context = GetDC(m_window_handle);

        ShowWindow(m_window_handle, SW_SHOWNORMAL);
    }

    void CWindowsWindow::SetTitle(CString title) {
        m_title = title;
        SetWindowTextA(m_window_handle, title.ToCString());
    }

    void CWindowsWindow::SetSize(Math::SVec2 size) {

    }

    void CWindowsWindow::SetWindowMode(EWindowMode window_mode) {
        if (m_window_mode == window_mode) {
            return;
        }

        DWORD window_style = GetWindowLongA(m_window_handle, GWL_STYLE);
        if (window_mode == EWindowMode::Borderless || window_mode == EWindowMode::Fullscreen) {
            MONITORINFO monitor_info = { sizeof(monitor_info) };
            GetWindowPlacement(m_window_handle, &m_previous_placement);
            GetMonitorInfoA(MonitorFromWindow(m_window_handle, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
            SetWindowLongA(m_window_handle, GWL_STYLE, window_style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(m_window_handle,
                HWND_TOP,
                monitor_info.rcMonitor.left,
                monitor_info.rcMonitor.top,
                monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        } else {
            // TODO: How do we handle styles correctly?
            SetWindowLongA(m_window_handle, GWL_STYLE, window_style | (WS_OVERLAPPEDWINDOW));
            SetWindowPlacement(m_window_handle, &m_previous_placement);
            u32 flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;
            SetWindowPos(m_window_handle, NULL, 0, 0, 0, 0, flags);
        }

        m_window_mode = window_mode;
    }

    void CWindowsWindow::SetVSyncMode(EVSyncMode vsync_mode) {

    }

    void CWindowsWindow::Update() const {
        SwapBuffers(m_device_context);

        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);

            if (message.message == WM_QUIT) {
                CApplication::GetInstance()->Exit();
            }
        }
    }

    LRESULT window_callback(HWND window_handle, u32 message, WPARAM first_message_param, LPARAM second_message_param) {
        LRESULT result = 0;

        switch (message) {
            case WM_CLOSE: {
                PostQuitMessage(0);
                break;
            }
            default: {
                result = DefWindowProcA(window_handle, message, first_message_param, second_message_param);
            }
        }

        return result;
    }
}