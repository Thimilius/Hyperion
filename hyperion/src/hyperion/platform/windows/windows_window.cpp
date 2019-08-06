#include "hyppch.hpp"

#include "windows_window.hpp"

#include "windows_opengl_graphics_context.hpp"

// TODO: Fully implement events

namespace Hyperion {

    CWindow *CWindow::Create(const CString &title, u32 width, u32 height, EWindowMode window_mode) {
        return new CWindowsWindow(title, width, height, window_mode);
    }

    CWindowsWindow::CWindowsWindow(const CString &title, u32 width, u32 height, EWindowMode window_mode) {
        m_title = title;
        m_width = width;
        m_height = height;
        m_window_mode = window_mode;

        u32 window_styles = WS_OVERLAPPEDWINDOW; //& ~WS_THICKFRAME & ~WS_MAXIMIZEBOX
        const char *window_class_name = "HYPERION_WINDOW_CLASS";
        HINSTANCE instance = GetModuleHandleA(NULL);

        WNDCLASSEXA window_class = { 0 };
        window_class.cbSize = sizeof(window_class);
        window_class.lpszClassName = window_class_name;
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.hInstance = instance;
        window_class.lpfnWndProc = &MessageCallback;
        window_class.hCursor = LoadCursorA(NULL, (LPSTR)IDC_ARROW);

        RegisterClassExA(&window_class);

        RECT window_rect = {0};
        window_rect.right = (LONG)m_width;
        window_rect.bottom = (LONG)m_height;
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
            this // Used later to store user pointer
        );

        if (m_window_handle == NULL) {
            HYP_CORE_ERROR("Failed to create window!");
        }

        SetLastError(0);
        auto result = SetWindowLongPtrA(m_window_handle, GWLP_USERDATA, (LONG_PTR)(void*)this);
        auto error = GetLastError();

        // TODO: We need to have a way to differentiate between graphics apis
        m_graphics_context = new Rendering::CWindowsOpenGLGraphicsContext(m_window_handle);
        m_graphics_context->Init();

        ShowWindow(m_window_handle, SW_SHOWNORMAL);
    }

    void CWindowsWindow::SetTitle(CString title) {
        m_title = title;
        SetWindowTextA(m_window_handle, title.ToCString());
    }

    void CWindowsWindow::SetSize(u32 width, u32 height) {

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
        if (m_vsync_mode != vsync_mode) {
            m_graphics_context->SetVSyncMode(vsync_mode);
        }
    }

    void CWindowsWindow::Update() const {
        m_graphics_context->SwapBuffers();

        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }

    void CWindowsWindow::DispatchEvent(CEvent &event) const {
        if (m_event_callback != nullptr) {
            m_event_callback(event);
        }
    }

    LRESULT CWindowsWindow::MessageCallback(HWND window_handle, u32 message, WPARAM w_param, LPARAM l_param) {
        LRESULT result = 0;

        // This will be null on creation
        CWindowsWindow *window = (CWindowsWindow*)GetWindowLongPtrA(window_handle, GWLP_USERDATA);

        switch (message) {
            case WM_CREATE: {
                // Store user pointer
                SetWindowLongPtrA(window_handle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT *)l_param)->lpCreateParams);
                SetWindowPos(window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
                break;
            }
            case WM_CLOSE: {
                CWindowCloseEvent event;
                window->DispatchEvent(event);
                break;
            }
            case WM_ACTIVATEAPP: {
                CWindowFocusEvent event((bool)w_param);
                window->DispatchEvent(event);
                break;
            }
            default: {
                result = DefWindowProcA(window_handle, message, w_param, l_param);
            }
        }

        return result;
    }
}