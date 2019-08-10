#include "hyppch.hpp"

#include "windows_window.hpp"

#include "windows_opengl_graphics_context.hpp"

namespace Hyperion {

    using namespace Events;

    CWindow *CWindow::Create(const CString &title, u32 width, u32 height, EWindowMode window_mode) {
        return new CWindowsWindow(title, width, height, window_mode);
    }

    CWindowsWindow::CWindowsWindow(const CString &title, u32 width, u32 height, EWindowMode window_mode) {
        m_title = title;
        m_width = width;
        m_height = height;
        m_window_state = EWindowState::Normal;

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

        SetWindowLongPtrA(m_window_handle, GWLP_USERDATA, (LONG_PTR)(void *)this);

        SetWindowMode(window_mode);

        CreateContext();
    }

    void CWindowsWindow::SetTitle(const CString &title) {
        m_title = title;
        SetWindowTextA(m_window_handle, title.ToCString());
    }

    void CWindowsWindow::SetSize(u32 width, u32 height) {
        m_width = width;
        m_height = height;

        RECT window_rect = { 0 };
        window_rect.right = (LONG)width;
        window_rect.bottom = (LONG)height;
        AdjustWindowRect(&window_rect, GetWindowLongA(m_window_handle, GWL_STYLE), false);
        width = window_rect.right - window_rect.left;
        height = window_rect.bottom - window_rect.top;

        u32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
        SetWindowPos(m_window_handle, NULL, 0, 0, width, height, flags);
    }

    void CWindowsWindow::SetWindowMode(EWindowMode window_mode) {
        if (m_window_mode == window_mode) {
            return;
        }

        DWORD window_styles = GetWindowLongA(m_window_handle, GWL_STYLE);

        EWindowMode last_window_mode = m_window_mode;
        // This already needs to be set here for all later message callbacks
        m_window_mode = window_mode;

        switch (window_mode) {
            case Hyperion::EWindowMode::Windowed: {
                ResetFullscreenDisplayMode();

                // TODO: How do we handle styles correctly?
                SetWindowLongA(m_window_handle, GWL_STYLE, window_styles | (WS_OVERLAPPEDWINDOW));
                SetWindowPlacement(m_window_handle, &m_previous_placement);
                u32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;

                RECT window_rect = { 0 };
                window_rect.right = (LONG)1280;
                window_rect.bottom = (LONG)720;
                AdjustWindowRect(&window_rect, window_styles, false);

                LONG width = window_rect.right - window_rect.left;
                LONG height = window_rect.bottom - window_rect.top;

                SetWindowPos(m_window_handle, NULL, 0, 0, width, height, flags);
                break;
            }
            case Hyperion::EWindowMode::Borderless: {
                if (last_window_mode == EWindowMode::Windowed) {
                    GetWindowPlacement(m_window_handle, &m_previous_placement);
                }

                ResetFullscreenDisplayMode();

                MONITORINFO monitor_info = { sizeof(monitor_info) };
                GetMonitorInfoA(MonitorFromWindow(m_window_handle, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
                SetWindowLongA(m_window_handle, GWL_STYLE, window_styles & ~WS_OVERLAPPEDWINDOW);

                LONG x = monitor_info.rcMonitor.left;
                LONG y = monitor_info.rcMonitor.top;
                LONG width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
                LONG height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

                SetWindowPos(m_window_handle, HWND_TOP, x, y, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
                break;
            }
            case Hyperion::EWindowMode::Fullscreen: {
                if (last_window_mode == EWindowMode::Windowed) {
                    GetWindowPlacement(m_window_handle, &m_previous_placement);
                }
                
                SetWindowLongA(m_window_handle, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
                SetWindowLongA(m_window_handle, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
                SetWindowPos(m_window_handle, HWND_TOPMOST, 0, 0, 1280, 720, SWP_SHOWWINDOW);

                SetFullscreenDisplayMode();

                ShowWindow(m_window_handle, SW_MAXIMIZE);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE;
        }

    }

    void CWindowsWindow::SetWindowState(EWindowState window_state) {
        switch (window_state) {
            case Hyperion::EWindowState::Normal: {
                ShowWindow(m_window_handle, SW_RESTORE);
                break;
            }
            case Hyperion::EWindowState::Minimized: {
                ShowWindow(m_window_handle, SW_MINIMIZE);
                break;
            }
            case Hyperion::EWindowState::Maximized: {
                ShowWindow(m_window_handle, SW_MAXIMIZE);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE;
        }

        m_window_state = window_state;
    }

    void CWindowsWindow::SetVSyncMode(EVSyncMode vsync_mode) {
        if (m_vsync_mode != vsync_mode) {
            m_vsync_mode = vsync_mode;
            m_graphics_context->SetVSyncMode(vsync_mode);
        }
    }

    void CWindowsWindow::SetIcon(const char *path) {
        HICON icon = (HICON)LoadImageA(NULL, path, IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
        SendMessageA(m_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessageA(m_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);

        // We also set the icon for the console window
        HWND console_window_handle = GetConsoleWindow();
        SendMessageA(console_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessageA(console_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
    }

    void CWindowsWindow::Update() const {
        m_graphics_context->SwapBuffers();

        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }

    void CWindowsWindow::Show() const {
        ShowWindow(m_window_handle, SW_SHOWNORMAL);
    }

    void CWindowsWindow::CreateContext() {
        switch (Rendering::CRenderAPI::GetAPI()) {
            case Rendering::ERenderAPI::OpenGL: {
                m_graphics_context = new Rendering::CWindowsOpenGLGraphicsContext(m_window_handle);
                break;
            }
                
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE;
        }
        
        m_graphics_context->Init();
    }

    void CWindowsWindow::DispatchEvent(CEvent &event) const {
        if (m_event_callback != nullptr) {
            m_event_callback(event);
        }
    }

    void CWindowsWindow::ResetFullscreenDisplayMode() {
        u32 flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;
        SetWindowPos(m_window_handle, HWND_NOTOPMOST, 0, 0, 0, 0, flags);

        ChangeDisplaySettingsA(NULL, CDS_RESET);
    }

    void CWindowsWindow::SetFullscreenDisplayMode() {
        CDisplayInfo::SDisplayModeInfo current_mode_info = CDisplay::GetCurrentDisplayModeInfo();

        DEVMODEA dev_mode = { 0 };
        dev_mode.dmSize = sizeof(dev_mode);
        dev_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL | DM_DISPLAYFREQUENCY;
        dev_mode.dmBitsPerPel = current_mode_info.bits_per_pixel;
        dev_mode.dmDisplayFrequency = current_mode_info.refresh_rate;
        dev_mode.dmPelsWidth = 1280;
        dev_mode.dmPelsHeight = 720;

        // TODO: Add ability to change from primary display to a different one
        LONG result = ChangeDisplaySettingsA(&dev_mode, CDS_FULLSCREEN);
    }

    EMouseButtonCode CWindowsWindow::GetMouseButtonCode(u32 code) const {
        switch (code) {
            case MK_LBUTTON: return EMouseButtonCode::Left;
            case MK_RBUTTON: return EMouseButtonCode::Right;
            case MK_MBUTTON: return EMouseButtonCode::Middle;

            case MK_XBUTTON1: return EMouseButtonCode::Button4;
            case MK_XBUTTON2: return EMouseButtonCode::Button5;

            default: return EMouseButtonCode::None;
        }
    }

    EKeyCode CWindowsWindow::GetKeyCode(u32 code) const {
        switch (code) {
            case '1': return EKeyCode::Alpha1;
            case '2': return EKeyCode::Alpha2;
            case '3': return EKeyCode::Alpha3;
            case '4': return EKeyCode::Alpha4;
            case '5': return EKeyCode::Alpha5;
            case '6': return EKeyCode::Alpha6;
            case '7': return EKeyCode::Alpha7;
            case '8': return EKeyCode::Alpha8;
            case '9': return EKeyCode::Alpha9;

            case 'A': return EKeyCode::A;
            case 'B': return EKeyCode::B;
            case 'C': return EKeyCode::C;
            case 'D': return EKeyCode::D;
            case 'E': return EKeyCode::E;
            case 'F': return EKeyCode::F;
            case 'G': return EKeyCode::G;
            case 'H': return EKeyCode::H;
            case 'I': return EKeyCode::I;
            case 'J': return EKeyCode::J;
            case 'K': return EKeyCode::K;
            case 'L': return EKeyCode::L;
            case 'M': return EKeyCode::M;
            case 'N': return EKeyCode::N;
            case 'O': return EKeyCode::O;
            case 'P': return EKeyCode::P;
            case 'Q': return EKeyCode::Q;
            case 'R': return EKeyCode::R;
            case 'S': return EKeyCode::S;
            case 'T': return EKeyCode::T;
            case 'U': return EKeyCode::U;
            case 'V': return EKeyCode::V;
            case 'W': return EKeyCode::W;
            case 'X': return EKeyCode::X;
            case 'Y': return EKeyCode::Y;
            case 'Z': return EKeyCode::Z;

            case VK_ESCAPE: return EKeyCode::Escape;
            case VK_BACK: return EKeyCode::Back;
            case VK_TAB: return EKeyCode::Tab;
            case VK_SPACE: return EKeyCode::Space;
            case VK_RETURN: return EKeyCode::Return;

            case VK_PRINT: return EKeyCode::Print;
            case VK_SCROLL: return EKeyCode::Scroll;
            case VK_PAUSE: return EKeyCode::Pause;

            case VK_INSERT: return EKeyCode::Insert;
            case VK_DELETE: return EKeyCode::Delete;
            case VK_HOME: return EKeyCode::Home;
            case VK_END: return EKeyCode::End;
            case VK_PRIOR: return EKeyCode::PageUp;
            case VK_NEXT: return EKeyCode::PageDown;

            case VK_OEM_PLUS: return EKeyCode::Plus;
            case VK_OEM_MINUS: return EKeyCode::Minus;
            case VK_OEM_PERIOD: return EKeyCode::Period;
            case VK_OEM_COMMA: return EKeyCode::Comma;

            case VK_UP: return EKeyCode::Up;
            case VK_DOWN: return EKeyCode::Down;
            case VK_LEFT: return EKeyCode::Left;
            case VK_RIGHT: return EKeyCode::Right;

            case VK_NUMLOCK: return EKeyCode::Numlock;
            case VK_CAPITAL: return EKeyCode::Capslock;

            case VK_LWIN: return EKeyCode::LeftSuper;
            case VK_RWIN: return EKeyCode::RightSuper;
            case VK_APPS: return EKeyCode::Application;

            case VK_CONTROL: return EKeyCode::Control;
            case VK_LCONTROL: return EKeyCode::LeftControl;
            case VK_RCONTROL: return EKeyCode::RightControl;

            case VK_SHIFT: return EKeyCode::Shift;
            case VK_LSHIFT: return EKeyCode::LeftShift;
            case VK_RSHIFT: return EKeyCode::RightShift;

            case VK_MENU: return EKeyCode::Alt;
            case VK_LMENU: return EKeyCode::LeftAlt;
            case VK_RMENU: return EKeyCode::RightAlt;

            case VK_F1: return EKeyCode::F1;
            case VK_F2: return EKeyCode::F2;
            case VK_F3: return EKeyCode::F3;
            case VK_F4: return EKeyCode::F4;
            case VK_F5: return EKeyCode::F5;
            case VK_F6: return EKeyCode::F6;
            case VK_F7: return EKeyCode::F7;
            case VK_F8: return EKeyCode::F8;
            case VK_F9: return EKeyCode::F9;
            case VK_F10: return EKeyCode::F10;
            case VK_F11: return EKeyCode::F11;
            case VK_F12: return EKeyCode::F12;
            case VK_F13: return EKeyCode::F13;
            case VK_F14: return EKeyCode::F14;
            case VK_F15: return EKeyCode::F15;
            case VK_F16: return EKeyCode::F16;
            case VK_F17: return EKeyCode::F17;
            case VK_F18: return EKeyCode::F18;
            case VK_F19: return EKeyCode::F19;
            case VK_F20: return EKeyCode::F20;
            case VK_F21: return EKeyCode::F21;
            case VK_F22: return EKeyCode::F22;
            case VK_F23: return EKeyCode::F23;
            case VK_F24: return EKeyCode::F24;

            case VK_NUMPAD0: return EKeyCode::Num0;
            case VK_NUMPAD1: return EKeyCode::Num1;
            case VK_NUMPAD2: return EKeyCode::Num2;
            case VK_NUMPAD3: return EKeyCode::Num3;
            case VK_NUMPAD4: return EKeyCode::Num4;
            case VK_NUMPAD5: return EKeyCode::Num5;
            case VK_NUMPAD6: return EKeyCode::Num6;
            case VK_NUMPAD7: return EKeyCode::Num7;
            case VK_NUMPAD8: return EKeyCode::Num8;
            case VK_NUMPAD9: return EKeyCode::Num9;

            case VK_ADD: return EKeyCode::NumAdd;
            case VK_SUBTRACT: return EKeyCode::NumSubtract;
            case VK_DIVIDE: return EKeyCode::NumDivide;
            case VK_MULTIPLY: return EKeyCode::NumMultiply;
            case VK_SEPARATOR: return EKeyCode::NumEnter;
            case VK_DECIMAL: return EKeyCode::NumComma;

            default: return EKeyCode::None;
        }
    }

    EKeyModifier CWindowsWindow::GetKeyModifier() const {
        EKeyModifier key_modifier = EKeyModifier::None;

        if (GetKeyState(VK_SHIFT) & 0x8000) {
            key_modifier |= EKeyModifier::Shift;
        }
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            key_modifier |= EKeyModifier::Control;
        }
        if (GetKeyState(VK_MENU) & 0x8000) {
            key_modifier |= EKeyModifier::Alt;
        }
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) {
            key_modifier |= EKeyModifier::Super;
        }
        if (GetKeyState(VK_CAPITAL) & 1) {
            key_modifier |= EKeyModifier::Capslock;
        }
        if (GetKeyState(VK_NUMLOCK) & 1) {
            key_modifier |= EKeyModifier::Numlock;
        }

        return key_modifier;
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

            case WM_DEVICECHANGE:
            case WM_DISPLAYCHANGE: {
                CAppDisplayChangeEvent event;
                window->DispatchEvent(event);
                break;
            }

            case WM_CHAR: 
            case WM_SYSCHAR: {
                u32 character = (u32)w_param;
                CKeyTypedEvent event(character, window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_KEYDOWN: 
            case WM_SYSKEYDOWN: {
                CKeyPressedEvent event(window->GetKeyCode((u32)w_param), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP: {
                CKeyReleasedEvent event(window->GetKeyCode(((u32)w_param)), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN: {
                CMouseButtonPressedEvent event(window->GetMouseButtonCode((u32)w_param), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_XBUTTONUP: {
                if (message == WM_LBUTTONUP) {
                    w_param = MK_LBUTTON;
                } else if (message == WM_RBUTTONUP) {
                    w_param = MK_RBUTTON;
                } else if (message == WM_MBUTTONUP) {
                    w_param = MK_MBUTTON;
                } else if (GET_XBUTTON_WPARAM(w_param) == XBUTTON1) {
                    w_param = MK_XBUTTON1;
                } else if (GET_XBUTTON_WPARAM(w_param) == XBUTTON2) {
                    w_param = MK_XBUTTON2;
                }

                CMouseButtonReleasedEvent event(window->GetMouseButtonCode((u32)w_param), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_MOUSEMOVE: {
                u32 x = LOWORD(l_param);
                u32 y = HIWORD(l_param);
                CMouseMovedEvent event((float)x, (float)y);
                window->DispatchEvent(event);
                break;
            }

            case WM_MOUSEWHEEL: {
                s32 scroll = GET_WHEEL_DELTA_WPARAM(w_param);
                CMouseScrolledEvent event(scroll < 0 ? -1.0f : 1.0f);
                window->DispatchEvent(event);
                break;
            };

            case WM_SIZE: {
                EWindowState window_state;
                switch (w_param) {
                    case SIZE_RESTORED: window_state = EWindowState::Normal; break;
                    case SIZE_MINIMIZED: window_state = EWindowState::Minimized; break;
                    case SIZE_MAXIMIZED: window_state = EWindowState::Maximized; break;
                    default: window_state = EWindowState::Normal; break;
                }
                
                u32 width = LOWORD(l_param);
                u32 height = HIWORD(l_param);

                window->m_width = width;
                window->m_height = height;
                window->m_window_state = window_state;

                if (window->m_window_mode == EWindowMode::Fullscreen) {
                    if (window_state == EWindowState::Minimized) {
                        window->ResetFullscreenDisplayMode();
                    } else if (window_state == EWindowState::Normal || window_state == EWindowState::Maximized) {
                        window->SetFullscreenDisplayMode();
                    }
                }

                CWindowResizeEvent event(width, height);
                window->DispatchEvent(event);
                break;
            }

            case WM_KILLFOCUS: {
                if (window->m_window_mode == EWindowMode::Fullscreen) {
                    window->SetWindowState(EWindowState::Minimized);
                }
            }

            case WM_MOVE: {
                CWindowMovedEvent event;
                window->DispatchEvent(event);
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