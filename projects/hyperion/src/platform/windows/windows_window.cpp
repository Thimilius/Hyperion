#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_window.hpp"

#include "hyperion/core/app/events/event.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/platform/windows/windows_opengl_graphics_context.hpp"

namespace Hyperion {

    Window *Window::Create(const String &title, u32 width, u32 height, WindowMode window_mode, VSyncMode vsync_mode, Rendering::RenderBackend backend_api) {
        return new WindowsWindow(title, width, height, window_mode, vsync_mode, backend_api);
    }

    WindowsWindow::WindowsWindow(const String &title, u32 width, u32 height, WindowMode window_mode, VSyncMode vsync_mode, Rendering::RenderBackend backend_api) {
        m_title = title;
        m_width = width;
        m_height = height;
        m_window_state = WindowState::Normal;

        u32 window_styles = WS_OVERLAPPEDWINDOW;
        auto window_class_name = L"HYPERION_WINDOW_CLASS";
        HINSTANCE instance = GetModuleHandleW(NULL);
        if (!instance) {
            HYP_PANIC_MESSAGE("Engine", "Failed to get windows application instance!");
        }

        WNDCLASSEXW window_class = { 0 };
        window_class.cbSize = sizeof(window_class);
        window_class.lpszClassName = window_class_name;
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.hInstance = instance;
        window_class.lpfnWndProc = &MessageCallback;
        window_class.hCursor = LoadCursorW(NULL, IDC_ARROW);

        if (!RegisterClassExW(&window_class)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to register windows window class!");
        }

        Vec2 size = GetActualWindowSize(width, height);

        m_window_handle = CreateWindowExW(
            0,
            window_class_name,
            StringUtils::Utf8ToUtf16(title).c_str(),
            window_styles,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            (u32)size.x,
            (u32)size.y,
            NULL,
            NULL,
            instance,
            this // Parameter to WM_CREATE that then stores the user pointer
        );

        if (m_window_handle == NULL) {
            HYP_PANIC_MESSAGE("Engine", "Failed to create window!");
        }

        if (!SetWindowLongPtrW(m_window_handle, GWLP_USERDATA, (LONG_PTR)(void *)this)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to set window attribute!");
        }
        
        SetWindowMode(window_mode);

        CreateContext(backend_api);

        SetVSyncMode(vsync_mode);
    }

    void WindowsWindow::SetTitle(const String &title) {
        m_title = title;
        if (!SetWindowTextW(m_window_handle, StringUtils::Utf8ToUtf16(title).c_str())) {
            HYP_PANIC_MESSAGE("Engine", "Failed to set window title!");
        }
    }

    void WindowsWindow::SetSize(u32 width, u32 height) {
        if (m_width == width && m_height == height) {
            return;
        }

        switch (m_window_mode) {
            case Hyperion::WindowMode::Windowed: {
                // If we are maximized we first restore the window to be normal
                if (m_window_state == WindowState::Maximized) {
                    SetWindowState(WindowState::Normal);
                }

                Vec2 size = GetActualWindowSize(width, height);

                u32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
                SetWindowPos(m_window_handle, NULL, 0, 0, (u32)size.x, (u32)size.y, flags);
                break;
            }
            case Hyperion::WindowMode::Borderless: {
                // In borderless mode we do not resize because we would probably not fill the screen anymore
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        m_width = width;
        m_height = height;
    }

    void WindowsWindow::SetWindowMode(WindowMode window_mode) {
        if (m_window_mode == window_mode) {
            return;
        }

        DWORD window_styles = GetWindowLongW(m_window_handle, GWL_STYLE);

        WindowMode last_window_mode = m_window_mode;
        // This already needs to be set here for all later message callbacks
        m_window_mode = window_mode;

        switch (window_mode) {
            case Hyperion::WindowMode::Windowed: {
                SetWindowLongW(m_window_handle, GWL_STYLE, window_styles | (WS_OVERLAPPEDWINDOW));
                SetWindowPlacement(m_window_handle, &m_previous_placement);

                Vec2 size = GetActualWindowSize(m_width, m_height);

                u32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;
                SetWindowPos(m_window_handle, NULL, 0, 0, (u32)size.x, (u32)size.y, flags);
                break;
            }
            case Hyperion::WindowMode::Borderless: {
                GetWindowPlacement(m_window_handle, &m_previous_placement);

                MONITORINFO monitor_info = { sizeof(monitor_info) };
                GetMonitorInfoW(MonitorFromWindow(m_window_handle, MONITOR_DEFAULTTOPRIMARY), &monitor_info);
                SetWindowLongW(m_window_handle, GWL_STYLE, window_styles & ~WS_OVERLAPPEDWINDOW);

                LONG x = monitor_info.rcMonitor.left;
                LONG y = monitor_info.rcMonitor.top;
                LONG width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
                LONG height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;

                SetWindowPos(m_window_handle, HWND_TOP, x, y, width, height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

    }

    void WindowsWindow::SetWindowState(WindowState window_state) {
        if (m_window_state == window_state) {
            return;
        }

        bool result = true;
        switch (window_state) {
            case Hyperion::WindowState::Normal: {
                result = ShowWindow(m_window_handle, SW_RESTORE);
                break;
            }
            case Hyperion::WindowState::Minimized: {
                result = ShowWindow(m_window_handle, SW_MINIMIZE);
                break;
            }
            case Hyperion::WindowState::Maximized: {
                // We only maximize in windowed mode
                if (m_window_mode == WindowMode::Windowed) {
                    result = ShowWindow(m_window_handle, SW_MAXIMIZE);
                }
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        if (!result) {
            HYP_PANIC_MESSAGE("Engine", "Failed set window state!");
        }

        m_window_state = window_state;
    }

    void WindowsWindow::SetVSyncMode(VSyncMode vsync_mode) {
        if (m_vsync_mode != vsync_mode) {
            m_vsync_mode = vsync_mode;
            m_graphics_context->SetVSyncMode(vsync_mode);
        }
    }

    void WindowsWindow::SetIcon(const String &path) {
        HICON icon = (HICON)LoadImageW(NULL, StringUtils::Utf8ToUtf16(path).c_str(), IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
        SendMessageW(m_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessageW(m_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);

        // We also set the icon for the console window
        HWND console_window_handle = GetConsoleWindow();
        if (console_window_handle) {
            SendMessageW(console_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
            SendMessageW(console_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
        }
    }

    void WindowsWindow::Update() const {
        m_graphics_context->SwapBuffers();

        MSG message;
        while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    void WindowsWindow::Show() const {
        ShowWindow(m_window_handle, SW_SHOWNORMAL);
    }

    Vec2 WindowsWindow::GetActualWindowSize(u32 client_width, u32 client_height) {
        RECT window_rect = { 0 };
        window_rect.right = (LONG)client_width;
        window_rect.bottom = (LONG)client_height;
        if (!AdjustWindowRect(&window_rect, GetWindowLongW(m_window_handle, GWL_STYLE), false)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to calculate window size!");
        }
        return Vec2((f32)(window_rect.right - window_rect.left), (f32)(window_rect.bottom - window_rect.top));
    }

    MouseButtonCode WindowsWindow::TranslateMouseButtonCode(u32 code) const {
        code = code & ~(MK_CONTROL & MK_SHIFT);

        switch (code) {
            case MK_LBUTTON: return MouseButtonCode::Left;
            case MK_RBUTTON: return MouseButtonCode::Right;
            case MK_MBUTTON: return MouseButtonCode::Middle;

            case MK_XBUTTON1: return MouseButtonCode::Button4;
            case MK_XBUTTON2: return MouseButtonCode::Button5;

            default: return MouseButtonCode::None;
        }
    }

    KeyCode WindowsWindow::TranslateKeyCode(u32 w_param, u32 l_param) const {
        // Left and right keys need to be distinguished as extended keys
        if (w_param == VK_CONTROL) {
            if (l_param & 0x01000000) {
                return KeyCode::RightControl;
            }

            // Alt-Gr sends both left control and alt right messages.
            // We are only interested in the alt right message,
            // so we need to discard the left control message.
            DWORD message_time = GetMessageTime();
            MSG next_message;
            if (PeekMessageW(&next_message, NULL, 0, 0, PM_NOREMOVE)) {
                if (next_message.message == WM_KEYDOWN || next_message.message == WM_SYSKEYDOWN || next_message.message == WM_KEYUP || next_message.message == WM_SYSKEYUP) {
                    if (next_message.wParam == VK_MENU && (next_message.lParam & 0x01000000) && next_message.time == message_time) {
                        // Next message is right alt down so discard this
                        return KeyCode::None;
                    }
                }
            }

            return KeyCode::LeftControl;
        } else if (w_param == VK_SHIFT) {
            // Left and right shift keys are not send as extended keys
            // and therefore need to be queried explicitly
            bool left_shift_down = GetKeyState(VK_LSHIFT) & 0x8000;
            bool right_shift_down = GetAsyncKeyState(VK_RSHIFT) & 0x8000;

            if (left_shift_down) {
                return KeyCode::LeftShift;
            } else if (right_shift_down) {
                return KeyCode::RightShift;
            } else {
                // If neither the right nor the left shift key is down this means they just got released.
                // however we can not distinguish between the two and just send out both release events explicitly

                // FIXME: The event dispatching should not be done in this method
                KeyReleasedEvent left_event(KeyCode::LeftShift, GetKeyModifier());
                KeyReleasedEvent right_event(KeyCode::RightShift, GetKeyModifier());
                DispatchEvent(left_event);
                DispatchEvent(right_event);

                return KeyCode::None;
            }
        } else if (w_param == VK_MENU) {
            if (l_param & 0x01000000) {
                return KeyCode::RightAlt;
            } else {
                return KeyCode::LeftAlt;
            }
        }

        switch (w_param) {
            case '1': return KeyCode::Alpha1;
            case '2': return KeyCode::Alpha2;
            case '3': return KeyCode::Alpha3;
            case '4': return KeyCode::Alpha4;
            case '5': return KeyCode::Alpha5;
            case '6': return KeyCode::Alpha6;
            case '7': return KeyCode::Alpha7;
            case '8': return KeyCode::Alpha8;
            case '9': return KeyCode::Alpha9;

            case 'A': return KeyCode::A;
            case 'B': return KeyCode::B;
            case 'C': return KeyCode::C;
            case 'D': return KeyCode::D;
            case 'E': return KeyCode::E;
            case 'F': return KeyCode::F;
            case 'G': return KeyCode::G;
            case 'H': return KeyCode::H;
            case 'I': return KeyCode::I;
            case 'J': return KeyCode::J;
            case 'K': return KeyCode::K;
            case 'L': return KeyCode::L;
            case 'M': return KeyCode::M;
            case 'N': return KeyCode::N;
            case 'O': return KeyCode::O;
            case 'P': return KeyCode::P;
            case 'Q': return KeyCode::Q;
            case 'R': return KeyCode::R;
            case 'S': return KeyCode::S;
            case 'T': return KeyCode::T;
            case 'U': return KeyCode::U;
            case 'V': return KeyCode::V;
            case 'W': return KeyCode::W;
            case 'X': return KeyCode::X;
            case 'Y': return KeyCode::Y;
            case 'Z': return KeyCode::Z;

            case VK_ESCAPE: return KeyCode::Escape;
            case VK_BACK: return KeyCode::Back;
            case VK_TAB: return KeyCode::Tab;
            case VK_SPACE: return KeyCode::Space;
            case VK_RETURN: return KeyCode::Return;

            case VK_PRINT: return KeyCode::Print;
            case VK_SCROLL: return KeyCode::Scroll;
            case VK_PAUSE: return KeyCode::Pause;

            case VK_INSERT: return KeyCode::Insert;
            case VK_DELETE: return KeyCode::Delete;
            case VK_HOME: return KeyCode::Home;
            case VK_END: return KeyCode::End;
            case VK_PRIOR: return KeyCode::PageUp;
            case VK_NEXT: return KeyCode::PageDown;

            case VK_OEM_PLUS: return KeyCode::Plus;
            case VK_OEM_MINUS: return KeyCode::Minus;
            case VK_OEM_PERIOD: return KeyCode::Period;
            case VK_OEM_COMMA: return KeyCode::Comma;

            case VK_UP: return KeyCode::Up;
            case VK_DOWN: return KeyCode::Down;
            case VK_LEFT: return KeyCode::Left;
            case VK_RIGHT: return KeyCode::Right;

            case VK_NUMLOCK: return KeyCode::Numlock;
            case VK_CAPITAL: return KeyCode::Capslock;

            case VK_LWIN: return KeyCode::LeftSuper;
            case VK_RWIN: return KeyCode::RightSuper;
            case VK_APPS: return KeyCode::Application;

            case VK_F1: return KeyCode::F1;
            case VK_F2: return KeyCode::F2;
            case VK_F3: return KeyCode::F3;
            case VK_F4: return KeyCode::F4;
            case VK_F5: return KeyCode::F5;
            case VK_F6: return KeyCode::F6;
            case VK_F7: return KeyCode::F7;
            case VK_F8: return KeyCode::F8;
            case VK_F9: return KeyCode::F9;
            case VK_F10: return KeyCode::F10;
            case VK_F11: return KeyCode::F11;
            case VK_F12: return KeyCode::F12;
            case VK_F13: return KeyCode::F13;
            case VK_F14: return KeyCode::F14;
            case VK_F15: return KeyCode::F15;
            case VK_F16: return KeyCode::F16;
            case VK_F17: return KeyCode::F17;
            case VK_F18: return KeyCode::F18;
            case VK_F19: return KeyCode::F19;
            case VK_F20: return KeyCode::F20;
            case VK_F21: return KeyCode::F21;
            case VK_F22: return KeyCode::F22;
            case VK_F23: return KeyCode::F23;
            case VK_F24: return KeyCode::F24;

            case VK_NUMPAD0: return KeyCode::Num0;
            case VK_NUMPAD1: return KeyCode::Num1;
            case VK_NUMPAD2: return KeyCode::Num2;
            case VK_NUMPAD3: return KeyCode::Num3;
            case VK_NUMPAD4: return KeyCode::Num4;
            case VK_NUMPAD5: return KeyCode::Num5;
            case VK_NUMPAD6: return KeyCode::Num6;
            case VK_NUMPAD7: return KeyCode::Num7;
            case VK_NUMPAD8: return KeyCode::Num8;
            case VK_NUMPAD9: return KeyCode::Num9;

            case VK_ADD: return KeyCode::NumAdd;
            case VK_SUBTRACT: return KeyCode::NumSubtract;
            case VK_DIVIDE: return KeyCode::NumDivide;
            case VK_MULTIPLY: return KeyCode::NumMultiply;
            case VK_SEPARATOR: return KeyCode::NumEnter;
            case VK_DECIMAL: return KeyCode::NumComma;

            default: return KeyCode::None;
        }
    }

    KeyModifier WindowsWindow::GetKeyModifier() const {
        KeyModifier key_modifier = KeyModifier::None;

        if (GetKeyState(VK_SHIFT) & 0x8000) {
            key_modifier |= KeyModifier::Shift;
        }
        if (GetKeyState(VK_CONTROL) & 0x8000) {
            key_modifier |= KeyModifier::Control;
        }
        if (GetKeyState(VK_MENU) & 0x8000) {
            key_modifier |= KeyModifier::Alt;
        }
        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) {
            key_modifier |= KeyModifier::Super;
        }
        if (GetKeyState(VK_CAPITAL) & 1) {
            key_modifier |= KeyModifier::Capslock;
        }
        if (GetKeyState(VK_NUMLOCK) & 1) {
            key_modifier |= KeyModifier::Numlock;
        }

        return key_modifier;
    }

    void WindowsWindow::CreateContext(Rendering::RenderBackend backend_api) {
        switch (backend_api) {
            case Rendering::RenderBackend::OpenGL: {
                m_graphics_context.reset(new Rendering::WindowsOpenGLGraphicsContext(m_window_handle));
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        m_graphics_context->Init();
    }

    void WindowsWindow::DispatchEvent(Event &event) const {
        if (m_event_callback != nullptr) {
            m_event_callback(event);
        }
    }

    u32 WindowsWindow::GetMouseButtonFromMessage(u32 message, u32 w_param) const {
        if (message == WM_LBUTTONDOWN || message == WM_LBUTTONUP) {
            return MK_LBUTTON;
        } else if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP) {
            return MK_RBUTTON;
        } else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP) {
            return MK_MBUTTON;
        } else if (message == WM_XBUTTONDOWN || message == WM_XBUTTONUP) {
            if (GET_XBUTTON_WPARAM(w_param) == XBUTTON1) {
                return MK_XBUTTON2;
            } else if (GET_XBUTTON_WPARAM(w_param) == XBUTTON2) {
                return MK_XBUTTON1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    LRESULT WindowsWindow::MessageCallback(HWND window_handle, u32 message, WPARAM w_param, LPARAM l_param) {
        LRESULT result = 0;

        // This will be null on WM_CREATE
        WindowsWindow *window = (WindowsWindow*)GetWindowLongPtrW(window_handle, GWLP_USERDATA);

        switch (message) {
            case WM_CREATE: {
                // Store user pointer
                SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT *)l_param)->lpCreateParams);
                SetWindowPos(window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
                break;
            }

            case WM_DEVICECHANGE:
            case WM_DISPLAYCHANGE: {
                AppDisplayChangeEvent event;
                window->DispatchEvent(event);
                break;
            }

            case WM_CHAR: 
            case WM_SYSCHAR: {
                u32 character = (u32)w_param;
                KeyTypedEvent event(character, window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_KEYDOWN: 
            case WM_SYSKEYDOWN: {
                KeyCode key_code = window->TranslateKeyCode((u32)w_param, (u32)l_param);
                if (key_code != KeyCode::None) {
                    KeyPressedEvent event(key_code, window->GetKeyModifier());
                    window->DispatchEvent(event);
                }
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP: {
                KeyCode key_code = window->TranslateKeyCode((u32)w_param, (u32)l_param);
                if (key_code != KeyCode::None) {
                    KeyReleasedEvent event(key_code, window->GetKeyModifier());
                    window->DispatchEvent(event);
                }
                break;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN: {
                u32 code = window->GetMouseButtonFromMessage((u32)message, (u32)w_param);
                MouseButtonPressedEvent event(window->TranslateMouseButtonCode(code), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_XBUTTONUP: {
                u32 code = window->GetMouseButtonFromMessage((u32)message, (u32)w_param);
                MouseButtonReleasedEvent event(window->TranslateMouseButtonCode(code), window->GetKeyModifier());
                window->DispatchEvent(event);
                break;
            }

            case WM_MOUSEMOVE: {
                u32 x = LOWORD(l_param);
                u32 y = HIWORD(l_param);
                MouseMovedEvent event((f32)x, (f32)y);
                window->DispatchEvent(event);
                break;
            }

            case WM_MOUSEWHEEL: {
                s16 scroll = GET_WHEEL_DELTA_WPARAM(w_param);
                MouseScrolledEvent event(scroll / (f32)WHEEL_DELTA);
                window->DispatchEvent(event);
                break;
            };

            case WM_SIZE: {
                WindowState window_state;
                switch (w_param) {
                    case SIZE_RESTORED: window_state = WindowState::Normal; break;
                    case SIZE_MINIMIZED: window_state = WindowState::Minimized; break;
                    case SIZE_MAXIMIZED: window_state = WindowState::Maximized; break;
                    default: window_state = WindowState::Normal; break;
                }

                u32 width = LOWORD(l_param);
                u32 height = HIWORD(l_param);

                window->m_width = width;
                window->m_height = height;
                window->m_window_state = window_state;

                // We do not want to generate a window resize event when the size gets 0 because the window got minimized
                if (width > 0 && height > 0) {
                    WindowResizeEvent event(width, height);
                    window->DispatchEvent(event);
                }
                
                break;
            }

            case WM_MOVE: {
                WindowMovedEvent event;
                window->DispatchEvent(event);
                break;
            }

            case WM_CLOSE: {
                WindowCloseEvent event;
                window->DispatchEvent(event);
                break;
            }

            case WM_SETFOCUS: 
            case WM_KILLFOCUS: {
                WindowFocusEvent event(message == WM_SETFOCUS);
                window->DispatchEvent(event);
                break;
            }

            default: {
                result = DefWindowProcW(window_handle, message, w_param, l_param);
            }
        }

        return result;
    }
}