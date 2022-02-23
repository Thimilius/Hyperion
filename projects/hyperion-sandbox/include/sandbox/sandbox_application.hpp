//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//-------------------- Definition Namespace --------------------
namespace Sandbox {

  class SandboxApplication : public Hyperion::Application {
  protected:
    void OnSetup(Hyperion::ApplicationSettings &settings) override;
    void OnInitialize() override;
    void OnUpdate(float32 delta_time) override;
    void OnTick() override;
  private:
    void UpdateTitle();
  };

}