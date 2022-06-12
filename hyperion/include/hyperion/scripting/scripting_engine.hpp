//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_driver.hpp"
#include "hyperion/core/app/application_settings.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
  class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class ScriptingEngine final {
  private:
    ScriptingEngine() = delete;
    ~ScriptingEngine() = delete;
  private:
    static void Initialize(const ScriptingSettings &settings);
    static void PostInitialize();
    static void OnEngineModeChanged(EngineState old_state, EngineState new_state);
    static void Update();
    static void Shutdown();

    static IScriptingWorld *CreateWorld(World *world);
    static IScriptingWorld *CopyWorld(World *world, IScriptingWorld *scripting_world);
    static void DestroyWorld(IScriptingWorld *scripting_world); 
  private:
    inline static IScriptingDriver *s_scripting_driver;
  private:
    friend class Hyperion::Engine;
    friend class Hyperion::World;
  };

}
