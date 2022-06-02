//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_hierarchy.hpp"
#include "hyperion/ecs/entity/entity_manager.hpp"
#include "hyperion/ecs/world/world_environment.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Mesh;
  class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class World final {
  public:
    inline const String &GetName() const { return m_name; }
    inline void SetName(const String &name) { m_name = name; }
    inline WorldEnvironment &GetEnvironment() { return m_environment; }
    inline const WorldEnvironment &GetEnvironment() const { return m_environment; }

    inline EntityManager *GetEntityManager() { return &m_manager; }
    inline const EntityManager *GetEntityManager() const { return &m_manager; }
    
    inline EntityHierarchy *GetHierarchy() { return &m_hierarchy; }
    inline const EntityHierarchy *GetHierarchy() const { return &m_hierarchy; }

    inline Physics::IPhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }
  private:
    World();
    ~World();
  private:
    String m_name = "World";
    WorldEnvironment m_environment;

    EntityManager m_manager;
    EntityHierarchy m_hierarchy;

    Physics::IPhysicsWorld *m_physics_world;
  private:
    friend class Hyperion::WorldManager;
    friend class Hyperion::EntityManager;
    template<typename... Types>
    friend class Hyperion::EntityView;
  };

}
