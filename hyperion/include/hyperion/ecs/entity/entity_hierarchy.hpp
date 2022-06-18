//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class EntityManager;
  struct HierarchyComponent;
  class World;
  class WorldSerializer;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class EntityHierarchyTransformUpdate {
    None,
    Branch,
    All,
  };

  enum class EntityHierarchyDestructionPolicy {
    DestroyChildren,
    KeepChildrenAsRoots,
  };

  enum class EntityHierarchyInstantiationPolicy {
    KeepHierarchy,
    IgnoreHierarchy
  };
  
  class EntityHierarchy final {
  private:
    enum class EntityHierarchyRootPolicy {
      KeepChildren,
      RemoveChildren
    };
  public:
    World *GetWorld() const { return m_world; }
      
    EntityId GetRootCount() const { return m_root_count; }
    EntityId GetFirstRoot() const { return m_first_root; }
    EntityId GetLastRoot() const { return m_last_root; }

    void SetParent(EntityId entity, EntityId parent, EntityHierarchyTransformUpdate update = EntityHierarchyTransformUpdate::None);
    EntityId GetChild(EntityId parent, uint32 index);

    void UpdateTransform(EntityHierarchyTransformUpdate update = EntityHierarchyTransformUpdate::Branch, EntityId branch = EntityId::EMPTY);
  private:
    EntityHierarchy() = default;
  private:
    void HandleEntityCreation(EntityId entity);
    void HandleEntityInstantiation(EntityId original, EntityId instantiated);
    void HandleEntityInstantiationRecursive(EntityId original_parent, EntityId instantiated_parent);
    void HandleEntityDestruction(EntityId entity, EntityHierarchyDestructionPolicy destruction_policy);

    void RemoveOldRelations(EntityId entity, HierarchyComponent *entity_hierarchy);
    void AddRootRelation(EntityId entity, HierarchyComponent *entity_hierarchy, EntityHierarchyRootPolicy root_policy);
  private:
    World *m_world = nullptr;
    EntityManager *m_manager = nullptr;

    uint64 m_root_count = 0;
    EntityId m_first_root = EntityId::EMPTY;
    EntityId m_last_root = EntityId::EMPTY;
  private:
    friend class Hyperion::EntityManager;
    friend class Hyperion::World;
    friend class Hyperion::WorldSerializer;
  };

}
