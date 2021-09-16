//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/system/systems/transform_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // NOTE: For now we don't really do a lot of validation.
    // That includes trusting the entities have the proper components and that there are no circular references.

    //--------------------------------------------------------------
    void WorldHierarchy::SetParent(EntityId entity, EntityId parent, WorldHierarchyTransformUpdate update) {
        if (!m_world->IsValidId(entity)) {
            HYP_LOG_WARN("Entity", "Trying to set parent of nonexistent entity with id {}.", entity);
            return;
        }

        HierarchyComponent *entity_hierarchy = m_world->GetComponent<HierarchyComponent>(entity);
        HYP_ASSERT(entity_hierarchy);
        if (entity_hierarchy->parent == parent) {
            return;
        }

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
            m_roots.Remove(entity);
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

        // Add relation to new parent and siblings.
        if (parent == Entity::EMPTY) {
            AddRoot(entity, entity_hierarchy);
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
    void WorldHierarchy::UpdateTransform(WorldHierarchyTransformUpdate update, EntityId branch) {
        if (update == WorldHierarchyTransformUpdate::Branch) {
            if (m_world->IsValidId(branch)) {
                HierarchyComponent *branch_hierarchy = m_world->GetComponent<HierarchyComponent>(branch);
                DerivedTransformComponent *parent_derived_transform = nullptr;
                if (branch_hierarchy->parent != Entity::EMPTY) {
                    parent_derived_transform = m_world->GetComponent<DerivedTransformComponent>(branch_hierarchy->parent);
                }

                HierarchyTransformSystem hierarchy_transform_system;
                hierarchy_transform_system.UpdateBranch(m_world, branch, branch_hierarchy, parent_derived_transform);
            } else {
                HYP_LOG_WARN("Entity", "Trying to update transform of nonexistent entity with id {}.", branch);
            }
        } else if (update == WorldHierarchyTransformUpdate::All) {
            HierarchyTransformSystem hierarchy_transform_system;
            hierarchy_transform_system.Run(m_world);
        }
    }

    //--------------------------------------------------------------
    void WorldHierarchy::AddRoot(EntityId entity, HierarchyComponent *entity_hierarchy) {
        if (m_roots.IsEmpty()) {
            entity_hierarchy->previous_sibling = Entity::EMPTY;
        } else {
            EntityId old_last_root = m_roots.GetLast();
            HierarchyComponent *old_last_root_hierarchy = m_world->GetComponent<HierarchyComponent>(old_last_root);
            HYP_ASSERT(old_last_root_hierarchy);
            old_last_root_hierarchy->next_sibling = entity;
            entity_hierarchy->previous_sibling = old_last_root;
        }
        entity_hierarchy->parent = Entity::EMPTY;
        entity_hierarchy->next_sibling = Entity::EMPTY;
        m_roots.Add(entity);
    }

}