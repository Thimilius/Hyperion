//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/log.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/scripting/scripting_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class IScriptingDriver {
  public:
    virtual ~IScriptingDriver() = default;
  public:
    virtual void Initialize() = 0;
    virtual void PostInitialize() = 0;
    virtual void OnEngineModeChanged(EngineState old_state, EngineState new_state) = 0;
    virtual void Update() = 0;
    virtual void Shutdown() = 0;

    virtual IScriptingWorld *CreateWorld(World *world) = 0;
    virtual IScriptingWorld *CopyWorld(World *world, IScriptingWorld *scripting_world) = 0;
    virtual void DestroyWorld(IScriptingWorld *scripting_world) = 0;
  };

  class NullScriptingDriver final : public IScriptingDriver {
  public:
    void Initialize() override { HYP_LOG_INFO("Scripting", "Initialized Null scripting driver!"); }
    void PostInitialize() override { }
    void OnEngineModeChanged(EngineState old_state, EngineState new_state) override { }
    void Update() override { }
    void Shutdown() override { }
    
    IScriptingWorld *CreateWorld(World *world) override { return new NullScriptingWorld(); }
    IScriptingWorld *CopyWorld(World *world, IScriptingWorld *scripting_world) override { return new NullScriptingWorld(); }
    void DestroyWorld(IScriptingWorld *scripting_world) override { delete scripting_world; }
  };
    
}
