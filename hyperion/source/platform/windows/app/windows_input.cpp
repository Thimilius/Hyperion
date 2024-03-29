//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/app/windows_input.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>
#include <Xinput.h>

#undef LoadLibrary

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/system/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using XInputGetStateFunc = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_STATE *pState);
  using XInputSetStateFunc = DWORD(WINAPI *)(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration);

  //--------------------------------------------------------------
  DWORD WINAPI XInputGetStateStub(DWORD dwUserIndex, XINPUT_STATE *pState) {
    return ERROR_DEVICE_NOT_CONNECTED;
  }

  //--------------------------------------------------------------
  DWORD WINAPI XInputSetStateStub(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration) {
    return ERROR_DEVICE_NOT_CONNECTED;
  }

  LibraryHandle g_xinput_library;
  XInputGetStateFunc g_xinput_get_state = XInputGetStateStub;
  XInputSetStateFunc g_xinput_set_state = XInputSetStateStub;

  InputImplementation *Input::s_input_implementation = new WindowsInput();

  //--------------------------------------------------------------
  WindowsInput::WindowsInput() {
    g_xinput_library = OperatingSystem::LoadLibrary("xinput1_4.dll");
    if (!g_xinput_library) {
      g_xinput_library = OperatingSystem::LoadLibrary("xinput1_3.dll");
    }

    XInputGetStateFunc xinput_get_state = OperatingSystem::GetFunctionPointer<XInputGetStateFunc>(g_xinput_library, "XInputGetState");
    if (xinput_get_state) {
      g_xinput_get_state = xinput_get_state;
    }

    XInputSetStateFunc xinput_set_state = OperatingSystem::GetFunctionPointer<XInputSetStateFunc>(g_xinput_library, "XInputSetState");
    if (xinput_set_state) {
      g_xinput_set_state = xinput_set_state;
    }
  }

  //--------------------------------------------------------------
  WindowsInput::~WindowsInput() {
    OperatingSystem::UnloadLibrary(g_xinput_library);
  }

  //--------------------------------------------------------------
  void WindowsInput::SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) {
    uint32 gamepad_id = GetIdFromGamepad(gamepad);

    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = static_cast<WORD>(Math::Clamp01(left_vibration) * 65535.0f);
    vibration.wRightMotorSpeed = static_cast<WORD>(Math::Clamp01(left_vibration) * 65535.0f);

    g_xinput_set_state(gamepad_id, &vibration);
  }

  //--------------------------------------------------------------
  String WindowsInput::GetClipboard() const {
    String result;
    
    if (OpenClipboard(nullptr)) {
      HANDLE clipboard_data = GetClipboardData(CF_UNICODETEXT);
      if (clipboard_data != nullptr) {
        WCHAR *text_data = static_cast<WCHAR *>(GlobalLock(clipboard_data));
        if (text_data) {
          WideString wide_data = WideString(text_data);
          GlobalUnlock(clipboard_data);
          
          result = StringUtils::Utf16ToUtf8(wide_data);
        }
      }
      CloseClipboard();
    }
    
    return result;
  }
  
  //--------------------------------------------------------------
  void WindowsInput::OnAppEvent(AppEvent &app_event) {
    // We keep track of all input events in this frame.
    if (app_event.IsInCategory(AppEventCategory::Input)) {
      AppEventDispatcher dispatcher(app_event);
      dispatcher.Dispatch<KeyPressedAppEvent>([this](KeyPressedAppEvent &event) { m_events.Add(new KeyPressedAppEvent(event)); });
      dispatcher.Dispatch<KeyReleasedAppEvent>([this](KeyReleasedAppEvent &event) { m_events.Add(new KeyReleasedAppEvent(event)); });
      dispatcher.Dispatch<KeyTypedAppEvent>([this](KeyTypedAppEvent &event) { m_events.Add(new KeyTypedAppEvent(event)); });
      dispatcher.Dispatch<MouseButtonPressedAppEvent>([this](MouseButtonPressedAppEvent &event) { m_events.Add(new MouseButtonPressedAppEvent(event)); });
      dispatcher.Dispatch<MouseButtonReleasedAppEvent>([this](MouseButtonReleasedAppEvent &event) { m_events.Add(new MouseButtonReleasedAppEvent(event)); });
      dispatcher.Dispatch<MouseScrolledAppEvent>([this](MouseScrolledAppEvent &event) { m_events.Add(new MouseScrolledAppEvent(event)); });
      dispatcher.Dispatch<MouseMovedAppEvent>([this](MouseMovedAppEvent &event) { m_events.Add(new MouseMovedAppEvent(event)); });
    }

    AppEventDispatcher dispatcher(app_event);

    dispatcher.Dispatch<WindowFocusAppEvent>([this](WindowFocusAppEvent &window_focus_event) {
      Reset();
      m_gamepad_input_active = window_focus_event.GetFocus();
    });

    dispatcher.Dispatch<KeyPressedAppEvent>([this](KeyPressedAppEvent &key_pressed_event) {
      OnKeyEvent(key_pressed_event, true);
    });
    dispatcher.Dispatch<KeyReleasedAppEvent>([this](KeyReleasedAppEvent &key_released_event) {
      OnKeyEvent(key_released_event, false);
    });

    dispatcher.Dispatch<MouseButtonPressedAppEvent>([this](MouseButtonPressedAppEvent &mouse_button_pressed_event) {
      OnMouseButtonEvent(mouse_button_pressed_event, true);
    });
    dispatcher.Dispatch<MouseButtonReleasedAppEvent>([this](MouseButtonReleasedAppEvent &mouse_button_released_event) {
      OnMouseButtonEvent(mouse_button_released_event, false);
    });
    dispatcher.Dispatch<MouseScrolledAppEvent>([this](MouseScrolledAppEvent &mouse_scrolled_event) {
      m_mouse_scroll = mouse_scrolled_event.GetScroll();
    });
    dispatcher.Dispatch<MouseMovedAppEvent>([this](MouseMovedAppEvent &mouse_moved_event) {
      m_mouse_position = Vector2Int(mouse_moved_event.GetX(), mouse_moved_event.GetY());
    });
  }

  //--------------------------------------------------------------
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

      m_last_mouse_position = m_mouse_position;
      m_mouse_scroll = 0.0f;

      for (AppEvent *event : m_events) {
        delete event;
      }
      m_events.Clear();
    }

    if (m_gamepad_input_active) {
      for (DWORD i = 0; i < m_gamepads_connected.GetLength(); i++) {
        Gamepad gamepad = m_gamepads_connected[i];
        XINPUT_STATE state = { };
        DWORD result = g_xinput_get_state(GetIdFromGamepad(gamepad), &state);

        if (result != ERROR_SUCCESS) {
          // FIXME: Does this really work while we are iterating?
          if (m_gamepads_connected.Contains(gamepad)) {
            m_gamepads_connected.Remove(gamepad);

            GamepadConnectionChangedAppEvent event(gamepad, false);
            DispatchAppEvent(event);
          }
        }

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

        {
          float32 left_stick_x = (static_cast<float32>(state.Gamepad.sThumbLX) + 0.5f) / 32767.5f;
          float32 left_stick_y = (static_cast<float32>(state.Gamepad.sThumbLY) + 0.5f) / 32767.5f;
          float32 right_stick_x = (static_cast<float32>(state.Gamepad.sThumbRX) + 0.5f) / 32767.5f;
          float32 right_stick_y = (static_cast<float32>(state.Gamepad.sThumbRY) + 0.5f) / 32767.5f;
          float32 left_trigger = static_cast<float32>(state.Gamepad.bLeftTrigger) / 255.0f;
          float32 right_trigger = static_cast<float32>(state.Gamepad.bRightTrigger) / 255.0f;

          m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::LeftStick)] = ApplyGamepadDeadzone(left_stick_x, left_stick_y);
          m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::RightStick)] = ApplyGamepadDeadzone(right_stick_x, right_stick_y);
          // Left and right trigger are treated as if they had the same two x and y axes.
          m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::LeftTrigger)] = Vector2(left_trigger, left_trigger);
          m_gamepads[static_cast<int32>(gamepad)].axes[static_cast<int32>(GamepadAxis::RightTrigger)] = Vector2(right_trigger, right_trigger);
        }
      }
    }
  }

  //--------------------------------------------------------------
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
        gamepad.axes[j] = Vector2();
      }
    }

    for (AppEvent *event : m_events) {
      delete event;
    }
    m_events.Clear();
  }

  //--------------------------------------------------------------
  void WindowsInput::QueryConnectedGamepads() {
    for (DWORD gamepad_id = 0; gamepad_id < XUSER_MAX_COUNT; gamepad_id++) {
      XINPUT_STATE state = { };
      DWORD result = g_xinput_get_state(gamepad_id, &state);

      Gamepad gamepad = GetGamepadFromId(gamepad_id);

      if (result == ERROR_SUCCESS) {
        if (!m_gamepads_connected.Contains(gamepad)) {
          m_gamepads_connected.Add(gamepad);

          GamepadConnectionChangedAppEvent event(gamepad, true);
          DispatchAppEvent(event);
        }
      } else {
        if (m_gamepads_connected.Contains(gamepad)) {
          m_gamepads_connected.Remove(gamepad);

          GamepadConnectionChangedAppEvent event(gamepad, false);
          DispatchAppEvent(event);
        }
      }
    }
  }

  //--------------------------------------------------------------
  void WindowsInput::OnKeyEvent(KeyAppEvent &key_app_event, bool8 down) {
    int32 key_code = static_cast<int32>(key_app_event.GetKeyCode());
    m_keys_down[key_code] = !m_keys_last[key_code] && down;
    m_keys[key_code] = down;
    m_keys_up[key_code] = m_keys_last[key_code] && !down;
  }

  //--------------------------------------------------------------
  void WindowsInput::OnMouseButtonEvent(MouseButtonAppEvent &mouse_button_app_event, bool8 down) {
    int32 mouse_button_code = static_cast<int32>(mouse_button_app_event.GetMouseButtonCode());
    m_mouse_buttons_down[mouse_button_code] = !m_mouse_buttons_last[mouse_button_code] && down;
    m_mouse_buttons[mouse_button_code] = down;
    m_mouse_buttons_up[mouse_button_code] = m_mouse_buttons_last[mouse_button_code] && !down;
  }

  //--------------------------------------------------------------
  void WindowsInput::HandleGamepadButtonCode(Gamepad gamepad, GamepadButtonCode button_code, bool8 down) {
    m_gamepads[static_cast<int32>(gamepad)].buttons_down[static_cast<int32>(button_code)] = !m_gamepads[static_cast<int32>(gamepad)].buttons_last[static_cast<
      int32>(button_code)] && down;
    m_gamepads[static_cast<int32>(gamepad)].buttons[static_cast<int32>(button_code)] = down;
    if (down) {
      GamepadButtonPressedAppEvent event(gamepad, button_code);
      DispatchAppEvent(event);
    }
    if (m_gamepads[static_cast<int32>(gamepad)].buttons_last[static_cast<int32>(button_code)] && !down) {
      m_gamepads[static_cast<int32>(gamepad)].buttons_up[static_cast<int32>(button_code)] = true;

      GamepadButtonReleasedAppEvent event(gamepad, button_code);
      DispatchAppEvent(event);
    }
  }

  //--------------------------------------------------------------
  Gamepad WindowsInput::GetGamepadFromId(uint32 id) {
    switch (id) {
      case 0: return Gamepad::Gamepad1;
      case 1: return Gamepad::Gamepad2;
      case 2: return Gamepad::Gamepad3;
      case 3: return Gamepad::Gamepad4;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return Gamepad::Gamepad1;
    }
  }

  //--------------------------------------------------------------
  uint32 WindowsInput::GetIdFromGamepad(Gamepad gamepad) {
    switch (gamepad) {
      case Gamepad::Gamepad1: return 0;
      case Gamepad::Gamepad2: return 1;
      case Gamepad::Gamepad3: return 2;
      case Gamepad::Gamepad4: return 3;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 0;
    }
  }

  //--------------------------------------------------------------
  Vector2 WindowsInput::ApplyGamepadDeadzone(float32 x, float32 y) {
    // Deadzone logic from: https://www.gamasutra.com/blogs/JoshSutphin/20130416/190541/Doing_Thumbstick_Dead_Zones_Right.php
    float32 dead_zone = m_gamepad_dead_zone;
    Vector2 left_stick = Vector2(x, y);

    if (left_stick.Magnitude() < dead_zone) {
      return Vector2();
    } else {
      left_stick = left_stick.Normalized() * ((left_stick.Magnitude() - dead_zone) / (1 - dead_zone));
      left_stick.x = Math::Clamp(left_stick.x, -1.0f, 1.0f);
      left_stick.y = Math::Clamp(left_stick.y, -1.0f, 1.0f);
      return left_stick;
    }
  }

}
