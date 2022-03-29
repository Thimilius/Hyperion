//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/events/app_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class MouseButtonAppEvent : public AppEvent {
  public:
    virtual ~MouseButtonAppEvent() = default;
  public:
    MouseButtonAppEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : m_mouse_button_code(mouse_button_code), m_key_modifier(key_modifier) { }

    inline MouseButtonCode GetMouseButtonCode() const { return m_mouse_button_code; }
    inline bool8 HasKeyModifier(KeyModifier key_modifier) const { return (m_key_modifier & key_modifier) == key_modifier; }

    AppEventCategory GetCategory() const override { return AppEventCategory::Input | AppEventCategory::Mouse | AppEventCategory::MouseButton; }
  private:
    MouseButtonCode m_mouse_button_code;
    KeyModifier m_key_modifier;
  };

  class MouseButtonPressedAppEvent : public MouseButtonAppEvent {
  public:
    MouseButtonPressedAppEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : MouseButtonAppEvent(mouse_button_code, key_modifier) { }

    AppEventType GetType() const override { return AppEventType::MouseButtonPressed; }
  public:
    static AppEventType GetStaticType() { return AppEventType::MouseButtonPressed; }
  };

  class MouseButtonReleasedAppEvent : public MouseButtonAppEvent {
  public:
    MouseButtonReleasedAppEvent(MouseButtonCode mouse_button_code, KeyModifier key_modifier) : MouseButtonAppEvent(mouse_button_code, key_modifier) { }

    AppEventType GetType() const override { return AppEventType::MouseButtonReleased; }
  public:
    static AppEventType GetStaticType() { return AppEventType::MouseButtonReleased; }
  };

  class MouseMovedAppEvent : public AppEvent {
  public:
    MouseMovedAppEvent(uint32 x, uint32 y) : m_x(x), m_y(y) { }

    inline int32 GetX() const { return m_x; }
    inline int32 GetY() const { return m_y; }

    AppEventType GetType() const override { return AppEventType::MouseMoved; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Input | AppEventCategory::Mouse; }
  public:
    static AppEventType GetStaticType() { return AppEventType::MouseMoved; }
  private:
    int32 m_x;
    int32 m_y;
  };

  class MouseScrolledAppEvent : public AppEvent {
  public:
    MouseScrolledAppEvent(float32 scroll) : m_scroll(scroll) { }

    inline float32 GetScroll() const { return m_scroll; }

    AppEventType GetType() const override { return AppEventType::MouseScrolled; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Input | AppEventCategory::Mouse; }
  public:
    static AppEventType GetStaticType() { return AppEventType::MouseScrolled; }
  private:
    float32 m_scroll;
  };

}