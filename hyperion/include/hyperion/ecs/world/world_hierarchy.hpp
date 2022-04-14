//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  struct HierarchyComponent;
  class World;
  class WorldSerializer;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class WorldHierarchyTransformUpdate {
    None,
    Branch,
    All,
  };

  enum class WorldHierarchyDestructionPolicy {
    DestroyChildren,
    KeepChildrenAsRoots,
  };

  class WorldHierarchy final {
  private:
    enum class WorldHierarchyRootPolicy {
      KeepChildren,
      RemoveChildren
    };

  public:
    EntityId GetRootCount() const { return m_root_count; }
    EntityId GetFirstRoot() const { return m_first_root; }
    EntityId GetLastRoot() const { return m_last_root; }

    void SetParent(EntityId entity, EntityId parent, WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::None);
    EntityId GetChild(EntityId parent, uint32 index);

    void UpdateTransform(WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::Branch, EntityId branch = Entity::EMPTY);
  private:
    void HandleEntityCreation(EntityId entity);
    void HandleEntityDestruction(EntityId entity, WorldHierarchyDestructionPolicy destruction_policy);

    void RemoveOldRelations(EntityId entity, HierarchyComponent *entity_hierarchy);
    void AddRootRelation(EntityId entity, HierarchyComponent *entity_hierarchy, WorldHierarchyRootPolicy root_policy);
  private:
    World *m_world;

    uint64 m_root_count = 0;
    EntityId m_first_root = Entity::EMPTY;
    EntityId m_last_root = Entity::EMPTY;
  private:
    friend class Hyperion::World;
    friend class Hyperion::WorldSerializer;
  };

}
