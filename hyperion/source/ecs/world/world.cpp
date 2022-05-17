//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  World::World() {
    m_manager.m_world = this;
    m_hierarchy.m_world = this;
    m_hierarchy.m_manager = &m_manager;

    m_physics_world = Physics::PhysicsEngine::CreateWorld(this);
  }

}
