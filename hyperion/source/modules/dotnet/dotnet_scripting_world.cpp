﻿//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/dotnet/dotnet_scripting_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

  //--------------------------------------------------------------
  DotnetScriptingWorld::DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver) {
    m_world = world;
  }
  
  //--------------------------------------------------------------
  DotnetScriptingWorld::DotnetScriptingWorld(World *world, DotnetScriptingDriver *driver, DotnetScriptingWorld *other) {
    
  }
  
  //--------------------------------------------------------------
  DotnetScriptingWorld::~DotnetScriptingWorld() {
    if (Engine::GetEngineState() != EngineState::Editor) {
      OnUnloadContext();
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnLoadContext() {
    // The destruction of entities and removal of components needs to be forwarded to the managed side. 
    EntityManager *manager = m_world->GetEntityManager();
    manager->RegisterOnEntityDestroyed({ ConnectionArguments<&DotnetScriptingWorld::OnEntityDestroyed>, this });
    for (ComponentInfo info : ComponentRegistry::GetComponentInfos()) {
      // We skip components which are treated in a special way.
      if (info.id == ComponentRegistry::GetId<DisabledComponent>() || info.id == ComponentRegistry::GetId<StaticComponent>()) {
        continue;
      }
      
      manager->RegisterOnComponentRemoved(info.id, { ConnectionArguments<&DotnetScriptingWorld::OnComponentRemoved>, this });
    }
    
    // Managed World and EntityManager instances are always being created.
    m_world_handle = DotnetScriptingBindings::GetOrCreateManagedObject(DotnetScriptingBindings::GetSpecialType(SpecialType::World), m_world);
    m_entity_manager_handle = DotnetScriptingBindings::GetOrCreateManagedObject(
      DotnetScriptingBindings::GetSpecialType(SpecialType::EntityManager),
      m_world->GetEntityManager()
    );
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnUnloadContext() {
    DotnetScriptingBindings::DestroyManagedObject(m_world->GetEntityManager(), m_entity_manager_handle);
    m_entity_manager_handle = nullptr;
    DotnetScriptingBindings::DestroyManagedObject(m_world, m_world_handle);
    m_world_handle = nullptr;
    
    EntityManager *manager = m_world->GetEntityManager();
    manager->UnregisterOnEntityDestroyed({ ConnectionArguments<&DotnetScriptingWorld::OnEntityDestroyed>, this });
    for (ComponentInfo info : ComponentRegistry::GetComponentInfos()) {
      manager->UnregisterOnComponentRemoved(info.id, { ConnectionArguments<&DotnetScriptingWorld::OnComponentRemoved>, this });
    }
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnEntityDestroyed(EntityManager *entity_manager, EntityId id) {
    if (Engine::GetEngineState() == EngineState::Editor) {
      return;
    }

    DotnetScriptingBindings::GetManagedBindings()->on_entity_destroyed(m_entity_manager_handle, id);
  }

  //--------------------------------------------------------------
  void DotnetScriptingWorld::OnComponentRemoved(EntityManager *entity_manager, ComponentId component_id, EntityId id) {
    if (Engine::GetEngineState() == EngineState::Editor) {
      return;
    }

    ManagedHandle component_type_handle = DotnetScriptingBindings::GetManagedTypeForComponentId(component_id);
    // TEMP: This check should only be temporary as long as we do not have every native component type as a managed binding.
    if (component_type_handle != nullptr) {
      DotnetScriptingBindings::GetManagedBindings()->on_component_removed(m_entity_manager_handle, component_type_handle, id);
    }
  }

}
  