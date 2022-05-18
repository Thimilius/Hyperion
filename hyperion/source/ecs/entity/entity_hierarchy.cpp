//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/entity/entity_hierarchy.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/systems/transform_systems.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  // NOTE: For now we don't really do a lot of validation.
  // That includes trusting the entities have the proper components and that there are no circular references.

  //--------------------------------------------------------------
  void EntityHierarchy::SetParent(EntityId entity, EntityId parent, EntityHierarchyTransformUpdate update) {
    if (!m_manager->IsAlive(entity)) {
      HYP_LOG_WARN("Entity", "Trying to set parent of nonexistent entity with id {}.", entity);
      return;
    }

    HierarchyComponent *entity_hierarchy = m_manager->GetComponent<HierarchyComponent>(entity);
    HYP_ASSERT(entity_hierarchy);
    if (entity_hierarchy->parent == parent) {
      return;
    }

    RemoveOldRelations(entity, entity_hierarchy);

    // Add relation to new parent and siblings.
    if (parent == EntityId::EMPTY) {
      AddRootRelation(entity, entity_hierarchy, EntityHierarchyRootPolicy::KeepChildren);
    } else {
      HierarchyComponent *new_parent_hierarchy = m_manager->GetComponent<HierarchyComponent>(parent);
      HYP_ASSERT(new_parent_hierarchy);
      if (new_parent_hierarchy->child_count == 0) {
        HYP_ASSERT(new_parent_hierarchy->first_child == EntityId::EMPTY && new_parent_hierarchy->last_child == EntityId::EMPTY);
        new_parent_hierarchy->first_child = entity;
        entity_hierarchy->previous_sibling = EntityId::EMPTY;
      } else {
        HierarchyComponent *old_last_child_hierarchy = m_manager->GetComponent<HierarchyComponent>(new_parent_hierarchy->last_child);
        HYP_ASSERT(old_last_child_hierarchy);
        old_last_child_hierarchy->next_sibling = entity;
        entity_hierarchy->previous_sibling = new_parent_hierarchy->last_child;
      }
      entity_hierarchy->parent = parent;
      entity_hierarchy->next_sibling = EntityId::EMPTY;
      new_parent_hierarchy->last_child = entity;
      new_parent_hierarchy->child_count++;
    }

    UpdateTransform(update, entity);
  }

  //--------------------------------------------------------------
  EntityId EntityHierarchy::GetChild(EntityId parent, uint32 index) {
    if (!m_manager->IsAlive(parent)) {
      HYP_LOG_WARN("Entity", "Trying to get child of nonexistent entity with id {}.", parent);
      return EntityId::EMPTY;
    }

    HierarchyComponent *parent_hierarchy = m_manager->GetComponent<HierarchyComponent>(parent);
    HYP_ASSERT(parent_hierarchy);
    if (index < 0 || index >= parent_hierarchy->child_count) {
      return EntityId::EMPTY;
    }

    uint32 current_index = 0;
    EntityId current_child = parent_hierarchy->first_child;
    while (current_index < index) {
      HierarchyComponent *child_hierarchy = m_manager->GetComponent<HierarchyComponent>(current_child);
      HYP_ASSERT(child_hierarchy);
      current_child = child_hierarchy->next_sibling;
      current_index++;
    }

    return current_child;
  }

  //--------------------------------------------------------------
  void EntityHierarchy::UpdateTransform(EntityHierarchyTransformUpdate update, EntityId branch) {
    if (update == EntityHierarchyTransformUpdate::Branch) {
      if (!m_manager->IsAlive(branch)) {
        HYP_LOG_WARN("Entity", "Trying to update transform of nonexistent entity with id {}.", branch);
      }

      HierarchyComponent *branch_hierarchy = m_manager->GetComponent<HierarchyComponent>(branch);
      HYP_ASSERT(branch_hierarchy);
      DerivedTransformComponent *parent_derived_transform = nullptr;
      if (branch_hierarchy->parent != EntityId::EMPTY) {
        parent_derived_transform = m_manager->GetComponent<DerivedTransformComponent>(branch_hierarchy->parent);
        HYP_ASSERT(parent_derived_transform);
      }

      HierarchyTransformSystem::UpdateBranch(m_manager, branch, branch_hierarchy, parent_derived_transform);
    } else if (update == EntityHierarchyTransformUpdate::All) {
      HierarchyTransformSystem hierarchy_transform_system;
      hierarchy_transform_system.Run(m_manager);
    }
  }

  //--------------------------------------------------------------
  void EntityHierarchy::HandleEntityCreation(EntityId entity) {
    HierarchyComponent *hierarchy = m_manager->GetComponent<HierarchyComponent>(entity);
    if (hierarchy) {
      AddRootRelation(entity, hierarchy, EntityHierarchyRootPolicy::RemoveChildren);
    }
  }

  //--------------------------------------------------------------
  void EntityHierarchy::HandleEntityDestruction(EntityId entity, EntityHierarchyDestructionPolicy destruction_policy) {
    HierarchyComponent *hierarchy = m_manager->GetComponent<HierarchyComponent>(entity);
    if (hierarchy) {
      RemoveOldRelations(entity, hierarchy);
    }

    // NOTE: We have to be very careful while looping as we are changing the hierarchy within.

    EntityId child = hierarchy->first_child;
    uint64 child_count = hierarchy->child_count;
    for (uint64 i = 0; i < child_count; i++) {
      HierarchyComponent *child_hierarchy = m_manager->GetComponent<HierarchyComponent>(child);
      EntityId next_child = child_hierarchy->next_sibling;

      switch (destruction_policy) {
        case EntityHierarchyDestructionPolicy::DestroyChildren: m_manager->DestroyEntity(child);
          break;
        case EntityHierarchyDestructionPolicy::KeepChildrenAsRoots: SetParent(child, EntityId::EMPTY);
          break;
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
      }

      child = next_child;
    }

    // TODO: In case of EntityHierarchyDestructionPolicy::KeepChildrenAsRoots what should happen to the transform?
    // We may want to keep the current world transformation.
  }

  //--------------------------------------------------------------
  void EntityHierarchy::RemoveOldRelations(EntityId entity, HierarchyComponent *entity_hierarchy) {
    // Remove relation to old siblings.
    if (entity_hierarchy->previous_sibling != EntityId::EMPTY) {
      HierarchyComponent *old_previous_sibling_hierarchy = m_manager->GetComponent<HierarchyComponent>(entity_hierarchy->previous_sibling);
      HYP_ASSERT(old_previous_sibling_hierarchy);
      old_previous_sibling_hierarchy->next_sibling = entity_hierarchy->next_sibling;
    }
    if (entity_hierarchy->next_sibling != EntityId::EMPTY) {
      HierarchyComponent *old_next_sibling_hierarchy = m_manager->GetComponent<HierarchyComponent>(entity_hierarchy->next_sibling);
      HYP_ASSERT(old_next_sibling_hierarchy);
      old_next_sibling_hierarchy->previous_sibling = entity_hierarchy->previous_sibling;
    }

    // Remove relation to old parent.
    if (entity_hierarchy->parent == EntityId::EMPTY) {
      if (m_first_root == entity) {
        m_first_root = entity_hierarchy->next_sibling;
      }
      if (m_last_root == entity) {
        m_last_root = entity_hierarchy->previous_sibling;
      }
      m_root_count--;
    } else {
      HierarchyComponent *old_parent_hierarchy = m_manager->GetComponent<HierarchyComponent>(entity_hierarchy->parent);
      HYP_ASSERT(old_parent_hierarchy);
      if (old_parent_hierarchy->first_child == entity) {
        old_parent_hierarchy->first_child = entity_hierarchy->next_sibling;
      }
      if (old_parent_hierarchy->last_child == entity) {
        old_parent_hierarchy->last_child = entity_hierarchy->previous_sibling;
      }
      old_parent_hierarchy->child_count--;
    }
  }

  //--------------------------------------------------------------
  void EntityHierarchy::AddRootRelation(EntityId entity, HierarchyComponent *entity_hierarchy, EntityHierarchyRootPolicy root_policy) {
    if (m_root_count == 0) {
      m_first_root = entity;
      entity_hierarchy->previous_sibling = EntityId::EMPTY;
    } else {
      HierarchyComponent *old_last_root_hierarchy = m_manager->GetComponent<HierarchyComponent>(m_last_root);
      HYP_ASSERT(old_last_root_hierarchy);
      old_last_root_hierarchy->next_sibling = entity;
      entity_hierarchy->previous_sibling = m_last_root;
    }
    entity_hierarchy->parent = EntityId::EMPTY;
    entity_hierarchy->next_sibling = EntityId::EMPTY;
    m_last_root = entity;
    m_root_count++;

    if (root_policy == EntityHierarchyRootPolicy::RemoveChildren) {
      entity_hierarchy->child_count = 0;
      entity_hierarchy->first_child = EntityId::EMPTY;
      entity_hierarchy->last_child = EntityId::EMPTY;
    }
  }

}
