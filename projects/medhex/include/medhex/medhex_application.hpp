//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/application.hpp>

//-------------------- Definition Namespace --------------------
namespace Medhex {

  class MedhexApplication : public Hyperion::Application {
  protected:
    void OnSetup(Hyperion::ApplicationSettings &settings) override;
    void OnInitialize() override;
    void OnUpdate(float32 delta_time) override;
  private:
    void UpdateInput(float32 delta_time);
    void UpdateTitle();
  };

}