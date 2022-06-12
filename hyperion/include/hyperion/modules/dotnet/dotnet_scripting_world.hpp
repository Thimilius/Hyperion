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
  private:
    void OnEntityDestroyed(EntityManager *entity_manager, EntityId id);
    void OnComponentRemoved(EntityManager *entity_manager, ComponentId component_id, EntityId id);
  private:
    World *m_world = nullptr;

    ManagedHandle m_world_handle = nullptr;
    ManagedHandle m_entity_managed_handle = nullptr;
  };
  
}
  