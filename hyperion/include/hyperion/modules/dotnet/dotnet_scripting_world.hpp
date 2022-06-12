//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/modules/dotnet/dotnet_scripting_driver.hpp"
#include "hyperion/scripting/scripting_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  class DotnetScriptingWorld final : public IScriptingWorld {
  public:
    DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver);
    DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver, DotnetScriptingWorld *other);
    ~DotnetScriptingWorld();
  };
  
}
  