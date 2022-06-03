//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class DotnetScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize() override;
    void PostInitialize() override;
    void OnEngineModeChanged(EngineState old_state, EngineState new_state) override;
    void Update() override;
    void Shutdown() override;
  private:
    void LoadManagedContext();
    void UnloadManagedContext();
  };
    
}
