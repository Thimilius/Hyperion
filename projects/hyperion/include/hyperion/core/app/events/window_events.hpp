//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/core/app/events/app_event.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class WindowCloseAppEvent : public AppEvent {
  public:
    AppEventType GetType() const override { return AppEventType::WindowClose; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Window; }
  public:
    static AppEventType GetStaticType() { return AppEventType::WindowClose; }
  };

  class WindowResizeAppEvent : public AppEvent {
  public:
    WindowResizeAppEvent(uint32 width, uint32 height) : m_width(width), m_height(height) { }

    inline uint32 GetWidth() const { return m_width; }
    inline uint32 GetHeight() const { return m_height; }

    AppEventType GetType() const override { return AppEventType::WindowResize; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Window; }
  public:
    static AppEventType GetStaticType() { return AppEventType::WindowResize; }
  private:
    uint32 m_width;
    uint32 m_height;
  };

  class WindowFocusAppEvent : public AppEvent {
  public:
    WindowFocusAppEvent(bool8 focus) : m_focus(focus) { }

    inline bool8 GetFocus() const { return m_focus; }

    AppEventType GetType() const override { return AppEventType::WindowFocus; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Window; }
  public:
    static AppEventType GetStaticType() { return AppEventType::WindowFocus; }
  private:
    bool8 m_focus;
  };

  class WindowMovedAppEvent : public AppEvent {
  public:
    AppEventType GetType() const override { return AppEventType::WindowMoved; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Window; }
  public:
    static AppEventType GetStaticType() { return AppEventType::WindowMoved; }
  };

  class WindowStateChangedAppEvent : public AppEvent {
  public:
    WindowStateChangedAppEvent(WindowState state) : m_state(state) { }

    inline WindowState GetState() const { return m_state; }

    AppEventType GetType() const override { return AppEventType::WindowStateChanged; }
    AppEventCategory GetCategory() const override { return AppEventCategory::Window; }
  public:
    static AppEventType GetStaticType() { return AppEventType::WindowStateChanged; }
  private:
    WindowState m_state;
  };

}