//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input_types.hpp"
#include "hyperion/core/app/events/app_event.hpp"
#include "hyperion/core/math/vector2.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class WindowsWindow;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class InputImplementation {
  public:
    virtual ~InputImplementation() = default;
  public:
    virtual bool8 IsKeyDown(KeyCode key_code) const = 0;
    virtual bool8 IsKeyHold(KeyCode key_code) const = 0;
    virtual bool8 IsKeyUp(KeyCode key_code) const = 0;
    virtual const Array<String> &GetKeysTyped() const = 0;

    virtual Vector2Int GetMousePosition() const = 0;
    virtual bool8 HasMouseMoved() const = 0;
    virtual float32 GetMouseScroll() const = 0;
    virtual bool8 HasMouseScrolled() const = 0;
    virtual bool8 IsMouseButtonDown(MouseButtonCode mouse_button_code) const = 0;
    virtual bool8 IsMouseButtonHold(MouseButtonCode mouse_button_code) const = 0;
    virtual bool8 IsMouseButtonUp(MouseButtonCode mouse_button_code) const = 0;

    virtual const Array<Gamepad> &GetConnectedGamepads() const = 0;
    virtual Vector2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) const = 0;
    virtual void SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) = 0;
    virtual float32 GetGamepadDeadZone() const = 0;
    virtual void SetGamepadDeadZone(float32 gamepad_dead_zone) = 0;
    virtual bool8 IsGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;
    virtual bool8 IsGamepadButtonHold(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;
    virtual bool8 IsGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) const = 0;

    virtual const Array<AppEvent *> &GetEvents() const = 0;
  protected:
    inline void DispatchAppEvent(AppEvent &app_event) const {
      if (m_app_event_callback) {
        m_app_event_callback(app_event);
      }
    }

    void SetEventCallback(const AppEventCallbackFunction &app_event_callback) { m_app_event_callback = app_event_callback; }
  private:
    AppEventCallbackFunction m_app_event_callback;
  };

  class Input final {
  public:
    inline static bool8 IsKeyDown(KeyCode key_code) { return s_input_implementation->IsKeyDown(key_code); }
    inline static bool8 IsKeyHold(KeyCode key_code) { return s_input_implementation->IsKeyHold(key_code); }
    inline static bool8 IsKeyUp(KeyCode key_code) { return s_input_implementation->IsKeyUp(key_code); }
    inline static const Array<String> &GetKeysTyped() { return s_input_implementation->GetKeysTyped(); }

    inline static Vector2Int GetMousePosition() { return s_input_implementation->GetMousePosition(); }
    inline static bool8 HasMouseMoved() { return s_input_implementation->HasMouseMoved(); }
    inline static float32 GetMouseScroll() { return s_input_implementation->GetMouseScroll(); }
    inline static bool8 HasMouseScrolled() { return s_input_implementation->HasMouseScrolled(); }
    inline static bool8 IsMouseButtonDown(MouseButtonCode mouse_button_code) { return s_input_implementation->IsMouseButtonDown(mouse_button_code); }
    inline static bool8 IsMouseButtonHold(MouseButtonCode mouse_button_code) { return s_input_implementation->IsMouseButtonHold(mouse_button_code); }
    inline static bool8 IsMouseButtonUp(MouseButtonCode mouse_button_code) { return s_input_implementation->IsMouseButtonUp(mouse_button_code); }

    inline static const Array<Gamepad> &GetConnectedGamepads() { return s_input_implementation->GetConnectedGamepads(); }
    inline static Vector2 GetGamepadAxis(Gamepad gamepad, GamepadAxis axis) { return s_input_implementation->GetGamepadAxis(gamepad, axis); }

    inline static void SetGamepadVibration(Gamepad gamepad, float32 left_vibration, float32 right_vibration) {
      s_input_implementation->SetGamepadVibration(gamepad, left_vibration, right_vibration);
    }

    inline static float32 GetGamepadDeadZone() { return s_input_implementation->GetGamepadDeadZone(); }
    inline static void GetGamepadDeadZone(float32 gamepad_dead_zone) { s_input_implementation->SetGamepadDeadZone(gamepad_dead_zone); }

    inline static bool8 IsGamepadButtonDown(Gamepad gamepad, GamepadButtonCode gamepad_button_code) {
      return s_input_implementation->IsGamepadButtonDown(gamepad, gamepad_button_code);
    }

    inline static bool8 IsGamepadButtonHold(Gamepad gamepad, GamepadButtonCode gamepad_button_code) {
      return s_input_implementation->IsGamepadButtonHold(gamepad, gamepad_button_code);
    }

    inline static bool8 IsGamepadButtonUp(Gamepad gamepad, GamepadButtonCode gamepad_button_code) {
      return s_input_implementation->IsGamepadButtonUp(gamepad, gamepad_button_code);
    }

    inline static const Array<AppEvent *> &GetEvents() { return s_input_implementation->GetEvents(); }
  private:
    Input() = delete;
    ~Input() = delete;
  private:
    static InputImplementation *s_input_implementation;
  private:
    friend class Hyperion::WindowsWindow;
  };

}
