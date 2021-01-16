#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_input.hpp"

#include <Windows.h>
#include <Xinput.h>

#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"

namespace Hyperion {

    using XInputGetStateFunc = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_STATE *pState);
    using XInputSetStateFunc = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);

    DWORD WINAPI XInputGetStateStub(DWORD dwUserIndex, XINPUT_STATE *pState) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    DWORD WINAPI XInputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) {
        return ERROR_DEVICE_NOT_CONNECTED;
    }

    HMODULE g_xinput_library;
    XInputGetStateFunc g_xinput_get_state = XInputGetStateStub;
    XInputSetStateFunc g_xinput_set_state = XInputSetStateStub;

    InputImplementation *Input::s_input_implementation = new WindowsInput();

    WindowsInput::WindowsInput() {
        // Try to load XInput as a dll
        g_xinput_library = LoadLibraryA("xinput1_4.dll");
        if (!g_xinput_library) {
            g_xinput_library = LoadLibraryA("xinput1_3.dll");
        }

        XInputGetStateFunc xinput_get_state = (XInputGetStateFunc)GetProcAddress(g_xinput_library, "XInputGetState");
        if (xinput_get_state) {
            g_xinput_get_state = xinput_get_state;
        }

        XInputSetStateFunc xinput_set_state = (XInputSetStateFunc)GetProcAddress(g_xinput_library, "XInputSetState");
        if (xinput_set_state) {
            g_xinput_set_state = xinput_set_state;
        }
    }

    WindowsInput::~WindowsInput() {
        FreeLibrary(g_xinput_library);
    }

    void WindowsInput::SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) {
        uint32 gamepad_id = GetIdFromGamepad(gamepad);

        XINPUT_VIBRATION vibration;
        vibration.wLeftMotorSpeed = (WORD)(Math::Clamp01(left_vibration) * 65535.0f);
        vibration.wRightMotorSpeed = (WORD)(Math::Clamp01(left_vibration) * 65535.0f);

        g_xinput_set_state(gamepad_id, &vibration);
    }

    void WindowsInput::OnEvent(Event &event) {
        EventDispatcher dispatcher(event);

        // Handle window events
        dispatcher.Dispatch<WindowFocusEvent>([this](WindowFocusEvent &window_focus_event){
            Reset();
            m_gamepad_input_active = window_focus_event.GetFocus();
        });

        // Handle key events
        dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent &key_pressed_event) {
            OnKeyEvent(key_pressed_event, true);
        });
        dispatcher.Dispatch<KeyReleasedEvent>([this](KeyReleasedEvent &key_released_event) {
            OnKeyEvent(key_released_event, false);
        });

        // Handle mouse events
        dispatcher.Dispatch<MouseScrolledEvent>([this](MouseScrolledEvent &mouse_scrolled_event) {
            m_mouse_scroll = mouse_scrolled_event.GetScroll();
        });
        dispatcher.Dispatch<MouseMovedEvent>([this](MouseMovedEvent &mouse_moved_event) {
            m_mouse_position = Vec2(mouse_moved_event.GetX(), mouse_moved_event.GetY());
        });
        dispatcher.Dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent &mouse_button_pressed_event) {
            OnMouseButtonEvent(mouse_button_pressed_event, true);
        });
        dispatcher.Dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent &mouse_button_released_event) {
            OnMouseButtonEvent(mouse_button_released_event, false);
        });
    }

    void WindowsInput::Update() {
        // Set input state for the new frame
        {
            memset(&m_keys_down, false, sizeof(m_keys_down));
            memset(&m_keys_up, false, sizeof(m_keys_up));
            memcpy(&m_keys_last, &m_keys, sizeof(m_keys_last));

            memset(&m_mouse_buttons_down, false, sizeof(m_mouse_buttons_down));
            memset(&m_mouse_buttons_up, false, sizeof(m_mouse_buttons_up));
            memcpy(&m_mouse_buttons_last, &m_mouse_buttons, sizeof(m_mouse_buttons_last));

            for (uint32 i = 0; i < static_cast<uint32>(Gamepad::Last); i++) {
                GamepadState &gamepad = m_gamepads[i];

                memset(&gamepad.buttons_down, false, sizeof(gamepad.buttons_down));
                memset(&gamepad.buttons_up, false, sizeof(gamepad.buttons_up));
                memcpy(&gamepad.buttons_last, &gamepad.buttons, sizeof(gamepad.buttons_last));
            }

            m_mouse_scroll = 0.0f;
        }
        
        // Query gamepad input
        if (m_gamepad_input_active) {
            for (DWORD i = 0; i < m_gamepads_connected.size(); i++) {
                Gamepad gamepad = m_gamepads_connected[i];
                XINPUT_STATE state = { 0 };
                DWORD result = g_xinput_get_state(GetIdFromGamepad(gamepad), &state);

                auto begin = m_gamepads_connected.begin();
                auto end = m_gamepads_connected.end();

                // Handle disconnection
                if (result != ERROR_SUCCESS) {
                    if (std::find(begin, end, gamepad) != end) {
                        m_gamepads_connected.erase(std::remove(begin, end, gamepad));

                        GamepadConnectionChangedEvent event(gamepad, false);
                        DispatchEvent(event);
                    }
                }

                // Handle buttons
                {
                    WORD buttons = state.Gamepad.wButtons;
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::Start, buttons & XINPUT_GAMEPAD_START);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::Back, buttons & XINPUT_GAMEPAD_BACK);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::A, buttons & XINPUT_GAMEPAD_A);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::B, buttons & XINPUT_GAMEPAD_B);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::X, buttons & XINPUT_GAMEPAD_X);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::Y, buttons & XINPUT_GAMEPAD_Y);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::DpadLeft, buttons & XINPUT_GAMEPAD_DPAD_LEFT);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::DpadRight, buttons & XINPUT_GAMEPAD_DPAD_RIGHT);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::DpadUp, buttons & XINPUT_GAMEPAD_DPAD_UP);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::DpadDown, buttons & XINPUT_GAMEPAD_DPAD_DOWN);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::LeftShoulder, buttons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::RightShoulder, buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::LeftThumb, buttons & XINPUT_GAMEPAD_LEFT_THUMB);
                    HandleGamepadButtonCode(gamepad, GamepadButtonCode::RightThumb, buttons & XINPUT_GAMEPAD_RIGHT_THUMB);
                }

                // Handle axes
                {
                    float32 left_stick_x = (state.Gamepad.sThumbLX + 0.5f) / 32767.5f;
                    float32 left_stick_y = (state.Gamepad.sThumbLY + 0.5f) / 32767.5f;
                    float32 right_stick_x = (state.Gamepad.sThumbRX + 0.5f) / 32767.5f;
                    float32 right_stick_y = (state.Gamepad.sThumbRY + 0.5f) / 32767.5f;
                    float32 left_trigger = state.Gamepad.bLeftTrigger / 255.0f;
                    float32 right_trigger = state.Gamepad.bRightTrigger / 255.0f;

                    m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::LeftStick)] = ApplyGamepadDeadzone(left_stick_x, left_stick_y);
                    m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::RightStick)] = ApplyGamepadDeadzone(right_stick_x, right_stick_y);
                    // Left and right trigger are treated as if they had the same two x and y axes
                    m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::LeftTrigger)] = Vec2(left_trigger, left_trigger);
                    m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::RightTrigger)] = Vec2(right_trigger, right_trigger);
                }
            }
        }
    }

    void WindowsInput::Reset() {
        memset(&m_keys_down, false, sizeof(m_keys_down));
        memset(&m_keys_up, false, sizeof(m_keys_up));
        memset(&m_keys, false, sizeof(m_keys));
        memset(&m_keys_last, false, sizeof(m_keys_last));

        m_mouse_scroll = 0.0f;

        memset(&m_mouse_buttons_down, false, sizeof(m_mouse_buttons_down));
        memset(&m_mouse_buttons_up, false, sizeof(m_mouse_buttons_up));
        memset(&m_mouse_buttons, false, sizeof(m_mouse_buttons));
        memset(&m_mouse_buttons_last, false, sizeof(m_mouse_buttons_last));

        for (uint32 i = 0; i < static_cast<uint32>(Gamepad::Last); i++) {
            GamepadState &gamepad = m_gamepads[i];

            memset(&gamepad.buttons_down, false, sizeof(gamepad.buttons_down));
            memset(&gamepad.buttons_up, false, sizeof(gamepad.buttons_up));
            memset(&gamepad.buttons, false, sizeof(gamepad.buttons));
            memset(&gamepad.buttons_last, false, sizeof(gamepad.buttons_last));

            for (uint32 j = 0; j < static_cast<uint32>(GamepadAxis::Last); j++) {
                gamepad.axes[j] = Vec2();
            }
        }
    }

    void WindowsInput::QueryConnectedGamepads() {
        for (DWORD gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++) {
            XINPUT_STATE state = { 0 };
            DWORD result = g_xinput_get_state(gamepad_id, &state);

            auto begin = m_gamepads_connected.begin();
            auto end = m_gamepads_connected.end();

            Gamepad gamepad = GetGamepadFromId(gamepad_id);

            if (result == ERROR_SUCCESS) {
                if (std::find(begin, end, gamepad) == end) {
                    m_gamepads_connected.push_back(gamepad);

                    GamepadConnectionChangedEvent event(gamepad, true);
                    DispatchEvent(event);
                }
            } else {
                if (std::find(begin, end, gamepad) != end) {
                    m_gamepads_connected.erase(std::remove(begin, end, gamepad));

                    GamepadConnectionChangedEvent event(gamepad, false);
                    DispatchEvent(event);
                }
            }
        }
    }

    void WindowsInput::OnKeyEvent(KeyEvent &event, bool down) {
        int32 key_code = static_cast<int32>(event.GetKeyCode());
        m_keys_down[key_code] = !m_keys_last[key_code] && down;
        m_keys[key_code] = down;
        m_keys_up[key_code] = m_keys_last[key_code] && !down;
    }

    void WindowsInput::OnMouseButtonEvent(MouseButtonEvent &event, bool down) {
        int32 mouse_button_code = static_cast<int32>(event.GetMouseButtonCode());
        m_mouse_buttons_down[mouse_button_code] = !m_mouse_buttons_last[mouse_button_code] && down;
        m_mouse_buttons[mouse_button_code] = down;
        m_mouse_buttons_up[mouse_button_code] = m_mouse_buttons_last[mouse_button_code] && !down;
    }

    void WindowsInput::HandleGamepadButtonCode(Gamepad gamepad, GamepadButtonCode button_code, bool down) {
        m_gamepads[static_cast<int32>(gamepad)].buttons_down[static_cast<int32>(button_code)] = !m_gamepads[static_cast<int32>(gamepad)].buttons_last[static_cast<int32>(button_code)] && down;
        m_gamepads[static_cast<int32>(gamepad)].buttons[static_cast<int32>(button_code)] = down;
        if (down) {
            GamepadButtonPressedEvent event(gamepad, button_code);
            DispatchEvent(event);
        }
        if (m_gamepads[static_cast<int32>(gamepad)].buttons_last[static_cast<int32>(button_code)] && !down) {
            m_gamepads[static_cast<int32>(gamepad)].buttons_up[static_cast<int32>(button_code)] = true;

            GamepadButtonReleasedEvent event(gamepad, button_code);
            DispatchEvent(event);
        }
    }

    Gamepad WindowsInput::GetGamepadFromId(uint32 id) {
        switch (id) {
            case 0: return Gamepad::Gamepad1;
            case 1: return Gamepad::Gamepad2;
            case 2: return Gamepad::Gamepad3;
            case 3: return Gamepad::Gamepad4;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Gamepad::Gamepad1;
        }
    }

    uint32 WindowsInput::GetIdFromGamepad(Gamepad gamepad) {
        switch (gamepad) {
            case Hyperion::Gamepad::Gamepad1: return 0;
            case Hyperion::Gamepad::Gamepad2: return 1;
            case Hyperion::Gamepad::Gamepad3: return 2;
            case Hyperion::Gamepad::Gamepad4: return 3;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    Vec2 WindowsInput::ApplyGamepadDeadzone(float32 x, float32 y) {
        // Deadzone logic from: https://www.gamasutra.com/blogs/JoshSutphin/20130416/190541/Doing_Thumbstick_Dead_Zones_Right.php
        float32 dead_zone = m_gamepad_dead_zone;
        Vec2 left_stick = Vec2(x, y);

        if (left_stick.Magnitude() < dead_zone) {
            return Vec2();
        } else {
            left_stick = left_stick.Normalized() * ((left_stick.Magnitude() - dead_zone) / (1 - dead_zone));
            left_stick.x = Math::Clamp(left_stick.x, -1.0f, 1.0f);
            left_stick.y = Math::Clamp(left_stick.y, -1.0f, 1.0f);
            return left_stick;
        }
    }

}