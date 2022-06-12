//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_engine.hpp"
#include "hyperion/scripting/scripting_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  World::World() {
    AssignInternalReferences();

    m_physics_world = Physics::PhysicsEngine::CreateWorld(this);
    m_scripting_world = Scripting::ScriptingEngine::CreateWorld(this);
  }

  //--------------------------------------------------------------
  World::~World() {
    Physics::PhysicsEngine::DestroyWorld(m_physics_world);
    Scripting::ScriptingEngine::DestroyWorld(m_scripting_world);
  }

  //--------------------------------------------------------------
  World::World(const World &other) {
    Copy(other);
  }
  
  //--------------------------------------------------------------
  World &World::operator=(const World &other) {
    if (this == &other) {
      return *this;
    }
    
    Copy(other);
    return *this;
  }
  
  //--------------------------------------------------------------
  void World::Copy(const World &other) {
    m_name = other.m_name;
    m_environment = other.m_environment;
    
    m_manager = other.m_manager;
    m_hierarchy = other.m_hierarchy;

    AssignInternalReferences();
    
    // FIXME: This should be handled by the copy constructor/assignment operator of the EntityManager.
    m_manager.m_entity_created_callbacks.Clear();
    m_manager.m_entity_destroyed_callbacks.Clear();
    for (ComponentCallbacks &component_callback : m_manager.m_storage.component_callbacks) {
      component_callback.added.Clear();
      component_callback.removed.Clear();
    }

    m_physics_world = Physics::PhysicsEngine::CopyWorld(this, other.m_physics_world);
    m_scripting_world = Scripting::ScriptingEngine::CopyWorld(this, other.m_scripting_world);
  }

  //--------------------------------------------------------------
  void World::AssignInternalReferences() {
    m_manager.m_world = this;
    m_hierarchy.m_world = this;
    m_hierarchy.m_manager = &m_manager;
  }

}
