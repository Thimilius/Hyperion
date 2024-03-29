//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Main;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Application {
  public:
    virtual ~Application() = default;
  public:
    inline Window *GetMainWindow() const { return m_main_window; }

    void Exit();
  public:
    inline static Application *GetInstance() { return s_instance; }
  protected:
    Application();

    virtual void OnRegisterTypes() { }
    virtual void OnSetup(ApplicationSettings &settings) { }
    virtual void OnInitialize() { }
    virtual void OnUpdate(float32 delta_time) { }
    virtual void OnFixedUpdate(float32 delta_time) { }
    virtual void OnTick() { }
    virtual void OnAppEvent(AppEvent &app_event) { }
    virtual void OnShutdown() { }
  private:
    Window *m_main_window;
  private:
    inline static Application *s_instance;
  private:
    friend class Hyperion::Engine;
  };

  // This is a function to be defined by the client...
  Application *CreateApplication();

}
