//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_world.hpp"

//---------------------- Project Includes ----------------------

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  //--------------------------------------------------------------
  DotnetScriptingWorld::DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver) {
    m_world = world;

    // The destruction of entities and removal of components needs to be forwarded to the managed side. 
    EntityManager *manager = world->GetEntityManager();
    manager->RegisterOnEntityDestroyed({ ConnectionArguments<&DotnetScriptingWorld::OnEntityDestroyed>, this });
    for (ComponentInfo info : ComponentRegistry::GetComponentInfos()) {
      manager->RegisterOnComponentRemoved(info.id, { ConnectionArguments<&DotnetScriptingWorld::OnComponentRemoved>, this });
    }

    // Managed World and EntityManager instances are always being created.
  }
  
  //--------------------------------------------------------------
  DotnetScriptingWorld::DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver, DotnetScriptingWorld *other) {
    
  }
  
  //--------------------------------------------------------------
  DotnetScriptingWorld::~DotnetScriptingWorld() {
    EntityManager *manager = m_world->GetEntityManager();
    manager->UnregisterOnEntityDestroyed({ ConnectionArguments<&DotnetScriptingWorld::OnEntityDestroyed>, this });
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnEntityDestroyed(EntityManager *entity_manager, EntityId id) {
    
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnComponentRemoved(EntityManager *entity_manager, ComponentId component_id, EntityId id) {
    
  }

}
  