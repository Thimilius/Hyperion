﻿//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"
#include "hyperion/modules/dotnet/dotnet_scripting_bindings.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  struct RuntimeNativeBindings {
    void (*exception)(ManagedString);
  };

  struct RuntimeManagedBindings {
    void (*load_context)(CoreBootstrapArguments *);
    void (*unload_context)();
  };

  struct RuntimeBoostrapArguments {
    RuntimeNativeBindings native_bindings;
    void (*managed_bindings_callback)(RuntimeManagedBindings *);
  };

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
  private:
    inline static RuntimeManagedBindings s_runtime_managed_bindings = { };
  };
    
}
