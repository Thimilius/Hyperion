//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/system/systems/transform_systems.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  // NOTE: For now we don't really do a lot of validation.
  // That includes trusting the entities have the proper components and that there are no circular references.

  //--------------------------------------------------------------
  void WorldHierarchy::SetParent(EntityId entity, EntityId parent, WorldHierarchyTransformUpdate update) {
    if (!m_world->IsAlive(entity)) {
      HYP_LOG_WARN("Entity", "Trying to set parent of nonexistent entity with id {}.", entity);
      return;
    }

    HierarchyComponent *entity_hierarchy = m_world->GetComponent<HierarchyComponent>(entity);
    HYP_ASSERT(entity_hierarchy);
    if (entity_hierarchy->parent == parent) {
      return;
    }

    RemoveOldRelations(entity, entity_hierarchy);

    // Add relation to new parent and siblings.
    if (parent == Entity::EMPTY) {
      AddRootRelation(entity, entity_hierarchy, WorldHierarchyRootPolicy::KeepChildren);
    } else {
      HierarchyComponent *new_parent_hierarchy = m_world->GetComponent<HierarchyComponent>(parent);
      HYP_ASSERT(new_parent_hierarchy);
      if (new_parent_hierarchy->child_count == 0) {
        HYP_ASSERT(new_parent_hierarchy->first_child == Entity::EMPTY && new_parent_hierarchy->last_child == Entity::EMPTY);
        new_parent_hierarchy->first_child = entity;
        entity_hierarchy->previous_sibling = Entity::EMPTY;
      } else {
        HierarchyComponent *old_last_child_hierarchy = m_world->GetComponent<HierarchyComponent>(new_parent_hierarchy->last_child);
        HYP_ASSERT(old_last_child_hierarchy);
        old_last_child_hierarchy->next_sibling = entity;
        entity_hierarchy->previous_sibling = new_parent_hierarchy->last_child;
      }
      entity_hierarchy->parent = parent;
      entity_hierarchy->next_sibling = Entity::EMPTY;
      new_parent_hierarchy->last_child = entity;
      new_parent_hierarchy->child_count++;
    }

    UpdateTransform(update, entity);
  }

  //--------------------------------------------------------------
  EntityId WorldHierarchy::GetChild(EntityId parent, uint32 index) {
    if (!m_world->IsAlive(parent)) {
      HYP_LOG_WARN("Entity", "Trying to get child of nonexistent entity with id {}.", parent);
      return Entity::EMPTY;
    }

    HierarchyComponent *parent_hierarchy = m_world->GetComponent<HierarchyComponent>(parent);
    HYP_ASSERT(parent_hierarchy);
    if (index < 0 || index >= parent_hierarchy->child_count) {
      return Entity::EMPTY;
    }

    uint32 current_index = 0;
    EntityId current_child = parent_hierarchy->first_child;
    while (current_index < index) {
      HierarchyComponent *child_hierarchy = m_world->GetComponent<HierarchyComponent>(current_child);
      HYP_ASSERT(child_hierarchy);
      current_child = child_hierarchy->next_sibling;
      current_index++;
    }

    return current_child;
  }

  //--------------------------------------------------------------
  void WorldHierarchy::UpdateTransform(WorldHierarchyTransformUpdate update, EntityId branch) {
    if (update == WorldHierarchyTransformUpdate::Branch) {
      if (!m_world->IsAlive(branch)) {
        HYP_LOG_WARN("Entity", "Trying to update transform of nonexistent entity with id {}.", branch);
      }

      HierarchyComponent *branch_hierarchy = m_world->GetComponent<HierarchyComponent>(branch);
      HYP_ASSERT(branch_hierarchy);
      DerivedTransformComponent *parent_derived_transform = nullptr;
      if (branch_hierarchy->parent != Entity::EMPTY) {
        parent_derived_transform = m_world->GetComponent<DerivedTransformComponent>(branch_hierarchy->parent);
        HYP_ASSERT(parent_derived_transform);
      }

      HierarchyTransformSystem::UpdateBranch(m_world, branch, branch_hierarchy, parent_derived_transform);
    } else if (update == WorldHierarchyTransformUpdate::All) {
      HierarchyTransformSystem hierarchy_transform_system;
      hierarchy_transform_system.Run(m_world);
    }
  }

  //--------------------------------------------------------------
  void WorldHierarchy::HandleEntityCreation(EntityId entity) {
    HierarchyComponent *hierarchy = m_world->GetComponent<HierarchyComponent>(entity);
    if (hierarchy) {
      AddRootRelation(entity, hierarchy, WorldHierarchyRootPolicy::RemoveChildren);
    }
  }

  //--------------------------------------------------------------
  void WorldHierarchy::HandleEntityDestruction(EntityId entity, WorldHierarchyDestructionPolicy destruction_policy) {
    HierarchyComponent *hierarchy = m_world->GetComponent<HierarchyComponent>(entity);
    if (hierarchy) {
      RemoveOldRelations(entity, hierarchy);
    }

    // NOTE: We have to be very careful while looping as we are changing the hierarchy within.

    EntityId child = hierarchy->first_child;
    uint64 child_count = hierarchy->child_count;
    for (uint64 i = 0; i < child_count; i++) {
      HierarchyComponent *child_hierarchy = m_world->GetComponent<HierarchyComponent>(child);
      EntityId next_child = child_hierarchy->next_sibling;

      switch (destruction_policy) {
        case WorldHierarchyDestructionPolicy::DestroyChildren: m_world->DestroyEntity(child);
          break;
        case WorldHierarchyDestructionPolicy::KeepChildrenAsRoots: SetParent(child, Entity::EMPTY);
          break;
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
      }

      child = next_child;
    }

    // TODO: In case of WorldHierarchyDestructionPolicy::KeepChildrenAsRoots what should happen to the transform.
    // We may want to keep the current world transformation.
  }

  //--------------------------------------------------------------
  void WorldHierarchy::RemoveOldRelations(EntityId entity, HierarchyComponent *entity_hierarchy) {
    // Remove relation to old siblings.
    if (entity_hierarchy->previous_sibling != Entity::EMPTY) {
      HierarchyComponent *old_previous_sibling_hierarchy = m_world->GetComponent<HierarchyComponent>(entity_hierarchy->previous_sibling);
      HYP_ASSERT(old_previous_sibling_hierarchy);
      old_previous_sibling_hierarchy->next_sibling = entity_hierarchy->next_sibling;
    }
    if (entity_hierarchy->next_sibling != Entity::EMPTY) {
      HierarchyComponent *old_next_sibling_hierarchy = m_world->GetComponent<HierarchyComponent>(entity_hierarchy->next_sibling);
      HYP_ASSERT(old_next_sibling_hierarchy);
      old_next_sibling_hierarchy->previous_sibling = entity_hierarchy->previous_sibling;
    }

    // Remove relation to old parent.
    if (entity_hierarchy->parent == Entity::EMPTY) {
      if (m_first_root == entity) {
        m_first_root = entity_hierarchy->next_sibling;
      }
      if (m_last_root == entity) {
        m_last_root = entity_hierarchy->previous_sibling;
      }
      m_root_count--;
    } else {
      HierarchyComponent *old_parent_hierarchy = m_world->GetComponent<HierarchyComponent>(entity_hierarchy->parent);
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
  void WorldHierarchy::AddRootRelation(EntityId entity, HierarchyComponent *entity_hierarchy, WorldHierarchyRootPolicy root_policy) {
    if (m_root_count == 0) {
      m_first_root = entity;
      entity_hierarchy->previous_sibling = Entity::EMPTY;
    } else {
      HierarchyComponent *old_last_root_hierarchy = m_world->GetComponent<HierarchyComponent>(m_last_root);
      HYP_ASSERT(old_last_root_hierarchy);
      old_last_root_hierarchy->next_sibling = entity;
      entity_hierarchy->previous_sibling = m_last_root;
    }
    entity_hierarchy->parent = Entity::EMPTY;
    entity_hierarchy->next_sibling = Entity::EMPTY;
    m_last_root = entity;
    m_root_count++;

    if (root_policy == WorldHierarchyRootPolicy::RemoveChildren) {
      entity_hierarchy->child_count = 0;
      entity_hierarchy->first_child = Entity::EMPTY;
      entity_hierarchy->last_child = Entity::EMPTY;
    }
  }

}
