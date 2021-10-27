//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/app/windows_window.hpp"

//---------------------- Library Includes ----------------------
#include <Dbt.h>
#include <Windowsx.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/engine.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/app/events/display_events.hpp"
#include "hyperion/core/app/events/gamepad_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/platform/windows/driver/windows_opengl_render_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    WindowsWindow::WindowsWindow(const WindowSettings &settings) {
        m_title = settings.title;
        m_width = settings.width;
        m_height = settings.height;
        m_min_width = settings.min_width;
        m_min_height = settings.min_height;
        m_window_state = WindowState::Normal;
        m_cursor_is_visible = true;
        m_cursor_mode = CursorMode::Default;

        m_input = static_cast<WindowsInput *>(Input::s_input_implementation);

        SetupWindow(settings);
        
        SetTitle(settings.title);
        SetIcon(settings.icon);
        SetWindowMode(settings.window_mode);

        m_input->QueryConnectedGamepads();
    }

    //--------------------------------------------------------------
    WindowsWindow::~WindowsWindow() {
        DestroyWindow(m_window_handle);
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetTitle(const String &title) {
        m_title = title;
        WideString title_wide = StringUtils::Utf8ToUtf16(title);
        const wchar_t *title_utf16 = title_wide.c_str();
        if (!SetWindowTextW(m_window_handle, title_utf16)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to set window title!");
        }
        if (!SetConsoleTitleW(title_utf16)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to set console window title!");
        }
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetSize(uint32 width, uint32 height) {
        if (m_width == width && m_height == height) {
            return;
        }

        switch (m_window_mode) {
            case WindowMode::Windowed: {
                // If we are maximized we first restore the window to be normal.
                if (m_window_state == WindowState::Maximized) {
                    SetWindowState(WindowState::Normal);
                }

                Vector2 size = GetActualWindowSize(width, height);

                uint32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
                SetWindowPos(m_window_handle, nullptr, 0, 0, static_cast<uint32>(size.x), static_cast<uint32>(size.y), flags);
                break;
            }
            case WindowMode::Borderless: {
                // In borderless mode we do not resize because we would probably not fill the screen anymore.
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        m_width = width;
        m_height = height;
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetMinimumSize(uint32 min_width, uint32 min_height) {
        m_min_width = min_width;
        m_min_height = min_height;
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetWindowMode(WindowMode window_mode) {
        if (m_window_mode == window_mode) {
            return;
        }

        DWORD window_styles = GetWindowLongW(m_window_handle, GWL_STYLE);

        WindowMode last_window_mode = m_window_mode;
        // This already needs to be set here for all later message callbacks.
        m_window_mode = window_mode;

        switch (window_mode) {
            case WindowMode::Windowed: {
                SetWindowLongW(m_window_handle, GWL_STYLE, window_styles | (WS_OVERLAPPEDWINDOW));
                SetWindowPlacement(m_window_handle, &m_previous_placement);

                Vector2 size = GetActualWindowSize(m_width, m_height);

                uint32 flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED;
                SetWindowPos(m_window_handle, nullptr, 0, 0, static_cast<uint32>(size.x), static_cast<uint32>(size.y), flags);
                break;
            }
            case WindowMode::Borderless: {
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

    //--------------------------------------------------------------
    void WindowsWindow::SetWindowState(WindowState window_state) {
        if (m_window_state == window_state) {
            return;
        }

        bool8 result = true;
        switch (window_state) {
            case WindowState::Normal: {
                result = ShowWindow(m_window_handle, SW_RESTORE);
                break;
            }
            case WindowState::Minimized: {
                result = ShowWindow(m_window_handle, SW_MINIMIZE);
                break;
            }
            case WindowState::Maximized: {
                // We only maximize in windowed mode.
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

    //--------------------------------------------------------------
    void WindowsWindow::SetCursorVisible(bool8 visible) {
        m_cursor_is_visible = visible;

        ShowCursor(visible);
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetCursorMode(CursorMode mode) {
        m_cursor_mode = mode;
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetIcon(const String &path) {
        HICON icon = (HICON)LoadImageW(nullptr, StringUtils::Utf8ToUtf16(path).c_str(), IMAGE_ICON, 64, 64, LR_LOADFROMFILE);
        SendMessageW(m_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        SendMessageW(m_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);

        // We also set the icon for the console window.
        HWND console_window_handle = GetConsoleWindow();
        if (console_window_handle) {
            SendMessageW(console_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)icon);
            SendMessageW(console_window_handle, WM_SETICON, ICON_BIG, (LPARAM)icon);
        }
    }

    //--------------------------------------------------------------
    Rendering::IRenderContext *WindowsWindow::CreateRenderDriverContext(Rendering::RenderBackend render_backend) {
        switch (render_backend) {
            case Rendering::RenderBackend::OpenGL: {
                // To create a proper OpenGL context we need a second helper window.
                const auto helper_window_class_name = L"HYPERION_HELPER_WINDOW_CLASS";
                HINSTANCE instance = GetModuleHandleW(nullptr);
                if (!instance) {
                    HYP_PANIC_MESSAGE("Engine", "Failed to get windows application instance!");
                }

                WNDCLASSEXW window_class = { 0 };
                window_class.cbSize = sizeof(window_class);
                window_class.lpszClassName = helper_window_class_name;
                window_class.style = CS_HREDRAW | CS_VREDRAW;
                window_class.hInstance = instance;
                window_class.lpfnWndProc = &DefWindowProcW;

                if (!RegisterClassExW(&window_class)) {
                    HYP_PANIC_MESSAGE("Engine", "Failed to register windows window class!");
                }

                HWND helper_window = CreateWindowExW(
                    0,
                    helper_window_class_name,
                    L"Hyperion helper window",
                    0,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    nullptr,
                    nullptr,
                    instance,
                    nullptr
                );

                Rendering::IRenderContext *render_driver_context = new Rendering::WindowsOpenGLRenderContext(GetDC(m_window_handle), GetDC(helper_window));

                // We can destroy the helper window now that we have the proper context.
                UnregisterClassW(helper_window_class_name, instance);
                DestroyWindow(helper_window);

                return render_driver_context;
            }
            case Rendering::RenderBackend::Vulkan: {
                return nullptr;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    void WindowsWindow::Poll() {
        m_input->Update();

        if (m_is_focused) {
            switch (m_cursor_mode) {
                case CursorMode::Default: break;
                case CursorMode::Confined: {
                    RECT window_rect;
                    GetWindowRect(m_window_handle, &window_rect);
                    ClipCursor(&window_rect);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }
        }

        MSG message;
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    //--------------------------------------------------------------
    void WindowsWindow::Show() {
        ShowWindow(m_window_handle, SW_SHOWNORMAL);
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetAppEventCallback(const AppEventCallbackFunction &app_event_callback) {
        m_app_event_callback = app_event_callback;
        m_input->SetEventCallback(app_event_callback);
    }

    //--------------------------------------------------------------
    void WindowsWindow::SetupWindow(const WindowSettings &settings) {
        uint32 window_styles = WS_OVERLAPPEDWINDOW;
        auto window_class_name = L"HYPERION_WINDOW_CLASS";
        HINSTANCE instance = GetModuleHandleW(nullptr);
        if (!instance) {
            HYP_PANIC_MESSAGE("Engine", "Failed to get windows application instance!");
        }

        WNDCLASSEXW window_class = { 0 };
        window_class.cbSize = sizeof(window_class);
        window_class.lpszClassName = window_class_name;
        window_class.style = CS_HREDRAW | CS_VREDRAW;
        window_class.hInstance = instance;
        window_class.lpfnWndProc = &MessageCallback;
        window_class.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        // NOTE: Having a null background is important as this prevents Windows from updating the window contents.
        // This allows to have proper rendering while resizing or moving the window.
        window_class.hbrBackground = nullptr; 

        if (!RegisterClassExW(&window_class)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to register windows window class!");
        }

        Vector2 size = GetActualWindowSize(settings.width, settings.height);

        m_window_handle = CreateWindowExW(
            0,
            window_class_name,
            StringUtils::Utf8ToUtf16(settings.title).c_str(),
            window_styles,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            static_cast<uint32>(size.x),
            static_cast<uint32>(size.y),
            nullptr,
            nullptr,
            instance,
            this
        );

        if (!m_window_handle) {
            HYP_PANIC_MESSAGE("Engine", "Failed to create window!");
        }

        if (!SetWindowLongPtrW(m_window_handle, GWLP_USERDATA, (LONG_PTR)(static_cast<void *>(this)))) {
            HYP_PANIC_MESSAGE("Engine", "Failed to set window attribute!");
        }
    }

    //--------------------------------------------------------------
    Vector2 WindowsWindow::GetActualWindowSize(uint32 client_width, uint32 client_height) const {
        RECT window_rect = { 0 };
        window_rect.right = static_cast<LONG>(client_width);
        window_rect.bottom = static_cast<LONG>(client_height);
        if (!AdjustWindowRect(&window_rect, GetWindowLongW(m_window_handle, GWL_STYLE), false)) {
            HYP_PANIC_MESSAGE("Engine", "Failed to calculate window size!");
        }
        return Vector2(static_cast<float32>(window_rect.right - window_rect.left), static_cast<float32>(window_rect.bottom - window_rect.top));
    }

    //--------------------------------------------------------------
    void WindowsWindow::DispatchAppEvent(AppEvent &app_event) const {
        m_input->OnAppEvent(app_event);

        if (m_app_event_callback) {
            m_app_event_callback(app_event);
        }
    }

    //--------------------------------------------------------------
    void WindowsWindow::DispatchKeyAppEvent(KeyCode key_code, bool8 is_down) const {
        if (key_code != KeyCode::None) {
            if (is_down) {
                KeyPressedAppEvent event(key_code, GetKeyModifier());
                DispatchAppEvent(event);
            } else {
                KeyReleasedAppEvent event(key_code, GetKeyModifier());
                DispatchAppEvent(event);
            }
        }
    }

    //--------------------------------------------------------------
    MouseButtonCode WindowsWindow::TranslateMouseButtonCode(uint32 code) const {
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

    //--------------------------------------------------------------
    KeyCode WindowsWindow::TranslateKeyCode(uint32 w_param, uint32 l_param, bool8 is_down) const {
        // Left and right keys need to be distinguished as extended keys.
        if (w_param == VK_CONTROL) {
            // Alt-Gr sends both left control and alt right messages.
            // We are only interested in the alt right message, so we need to discard the left control message.
            MSG next_message;
            if (PeekMessageW(&next_message, nullptr, 0, 0, PM_NOREMOVE)) {
                if (next_message.message == WM_KEYDOWN || next_message.message == WM_SYSKEYDOWN || next_message.message == WM_KEYUP || next_message.message == WM_SYSKEYUP) {
                    DWORD message_time = GetMessageTime();
                    if (next_message.wParam == VK_MENU && (next_message.lParam & 0x01000000) && next_message.time == message_time) {
                        DispatchKeyAppEvent(KeyCode::AltGr, is_down);

                        // Next message is right alt down so discard this.
                        return KeyCode::None;
                    }
                }
            }

            DispatchKeyAppEvent(KeyCode::Control, is_down);

            if (l_param & 0x01000000) {
                return KeyCode::RightControl;
            } else {
                return KeyCode::LeftControl;
            }
        } else if (w_param == VK_SHIFT) {
            DispatchKeyAppEvent(KeyCode::Shift, is_down);

            // Left and right shift keys are not send as extended keys and therefore need to be queried explicitly.
            bool8 previous_left_shift_down = m_left_shift_last_down;
            bool8 previous_right_shift_down = m_right_shift_last_down;
            m_left_shift_last_down = GetKeyState(VK_LSHIFT) & 0x8000;
            m_right_shift_last_down = GetAsyncKeyState(VK_RSHIFT) & 0x8000;

            if (m_left_shift_last_down) {
                return KeyCode::LeftShift;
            } else if (m_right_shift_last_down) {
                return KeyCode::RightShift;
            } else {
                // If neither the right nor the left shift key is down this means they just got released.
                // So send out the corresponding key released event.
                if (previous_left_shift_down) {
                    return KeyCode::LeftShift;
                } else if (previous_right_shift_down) {
                    return KeyCode::RightShift;
                } else {
                    return KeyCode::None;
                }
            }
        } else if (w_param == VK_MENU) {
            DispatchKeyAppEvent(KeyCode::Alt, is_down);

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

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
    uint32 WindowsWindow::GetMouseButtonFromMessage(uint32 message, uint32 w_param) const {
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

    //--------------------------------------------------------------
    LRESULT WindowsWindow::MessageCallback(HWND window_handle, uint32 message, WPARAM w_param, LPARAM l_param) {
        LRESULT result = 0;

        // This will be null on WM_CREATE.
        WindowsWindow *window = static_cast<WindowsWindow *>(reinterpret_cast<void *>(GetWindowLongPtrW(window_handle, GWLP_USERDATA)));

        switch (message) {
            case WM_CREATE: {
                SetWindowLongPtrW(window_handle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT *)l_param)->lpCreateParams);
                SetWindowPos(window_handle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
                break;
            }

            case WM_DEVICECHANGE: {
                if (w_param == DBT_DEVNODES_CHANGED) {
                    window->m_input->QueryConnectedGamepads();
                }
                break;
            }

            case WM_DISPLAYCHANGE: {
                DisplayChangeAppEvent event;
                window->DispatchAppEvent(event);
                break;
            }

            case WM_CHAR: 
            case WM_SYSCHAR: {
                uint32 character = static_cast<uint32>(w_param);
                KeyTypedAppEvent event(character, window->GetKeyModifier());
                window->DispatchAppEvent(event);
                break;
            }

            case WM_KEYDOWN: 
            case WM_SYSKEYDOWN: {
                KeyCode key_code = window->TranslateKeyCode(static_cast<uint32>(w_param), static_cast<uint32>(l_param), true);
                window->DispatchKeyAppEvent(key_code, true);
                break;
            }

            case WM_KEYUP:
            case WM_SYSKEYUP: {
                KeyCode key_code = window->TranslateKeyCode(static_cast<uint32>(w_param), static_cast<uint32>(l_param), false);
                window->DispatchKeyAppEvent(key_code, false);
                break;
            }

            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_XBUTTONDOWN: {
                SetCapture(window->m_window_handle);

                uint32 code = window->GetMouseButtonFromMessage(static_cast<uint32>(message), static_cast<uint32>(w_param));
                MouseButtonPressedAppEvent event(window->TranslateMouseButtonCode(code), window->GetKeyModifier());
                window->DispatchAppEvent(event);
                break;
            }

            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
            case WM_XBUTTONUP: {
                ReleaseCapture();
                
                uint32 code = window->GetMouseButtonFromMessage(static_cast<uint32>(message), static_cast<uint32>(w_param));
                MouseButtonReleasedAppEvent event(window->TranslateMouseButtonCode(code), window->GetKeyModifier());
                window->DispatchAppEvent(event);
                break;
            }

            case WM_MOUSEMOVE: {
                int32 x = GET_X_LPARAM(l_param);
                int32 y = GET_Y_LPARAM(l_param);
                
                int32 height = window->m_height;

                MouseMovedAppEvent event(x, height - y);
                window->DispatchAppEvent(event);
                break;
            }

            case WM_MOUSEWHEEL: {
                int16 scroll = GET_WHEEL_DELTA_WPARAM(w_param);
                MouseScrolledAppEvent event(scroll / static_cast<float32>(WHEEL_DELTA));
                window->DispatchAppEvent(event);
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

                uint32 width = LOWORD(l_param);
                uint32 height = HIWORD(l_param);

                window->m_width = width;
                window->m_height = height;
                window->m_window_state = window_state;

                // We do not want to generate a window resize event when the size gets 0 because the window got minimized.
                if (width > 0 && height > 0) {
                    WindowResizeAppEvent event(width, height);
                    window->DispatchAppEvent(event);
                }
                
                break;
            }

            case WM_GETMINMAXINFO: {
                MINMAXINFO *min_max_info = (MINMAXINFO *)l_param;
                // We need the check for the window pointer here because we get the GETMINMAXINFO message before the CREATE message.
                if (window) {
                    Vector2 min_size = window->GetActualWindowSize(window->m_min_width, window->m_min_height);
                    min_max_info->ptMinTrackSize.x = static_cast<LONG>(min_size.x);
                    min_max_info->ptMinTrackSize.y = static_cast<LONG>(min_size.y);
                }
                break;
            }

            case WM_MOVE: {
                WindowMovedAppEvent event;
                window->DispatchAppEvent(event);
                break;
            }

            case WM_CLOSE: {
                WindowCloseAppEvent event;
                window->DispatchAppEvent(event);
                break;
            }

            case WM_SETFOCUS: 
            case WM_KILLFOCUS: {
                bool8 focused = message == WM_SETFOCUS;
                window->m_is_focused = focused;

                // We need to reset the cursor clip state when losing focus.
                ClipCursor(nullptr);

                WindowFocusAppEvent event(focused);
                window->DispatchAppEvent(event);
                break;
            }

            case WM_ENTERSIZEMOVE: {
                window->m_timer = SetTimer(window->m_window_handle, 1, USER_TIMER_MINIMUM, nullptr);
                break;
            }
            case WM_EXITSIZEMOVE: {
                KillTimer(window->m_window_handle, window->m_timer);
                break;
            }
            case WM_TIMER: {
                if (w_param == window->m_timer) {
                    Engine::Iterate();
                }
                break;
            }

            default: {
                result = DefWindowProcW(window_handle, message, w_param, l_param);
            }
        }

        return result;
    }
}