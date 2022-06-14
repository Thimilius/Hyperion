//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"
#include "hyperion/modules/dotnet/dotnet_scripting_bindings.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Scripting {
  class DotnetScriptingWorld;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  struct RuntimeNativeBindings {
    void (*exception)(ManagedString);
  };

  struct RuntimeManagedBindings {
    void (*load_context)(LoadContextArguments *);
    void (*unload_context)();
  };

  struct RuntimeBoostrapArguments {
    RuntimeNativeBindings native_bindings;
    void (*managed_bindings_callback)(RuntimeManagedBindings *);
  };

  class DotnetScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize(const ScriptingSettings &settings) override;
    void PostInitialize() override;
    void OnEngineModeChanged(EngineState old_state, EngineState new_state) override;
    void Update() override;
    void Shutdown() override;
    
    IScriptingWorld *CreateWorld(World *world) override;
    IScriptingWorld *CopyWorld(World *world, IScriptingWorld *scripting_world) override;
    void DestroyWorld(IScriptingWorld *scripting_world) override;
  private:
    void LoadManagedContext();
    void UnloadManagedContext();
  private:
  private:
    inline static RuntimeManagedBindings s_runtime_managed_bindings = { };
    
    inline static Array<DotnetScriptingWorld *> s_scripting_worlds;
  };
    
}
