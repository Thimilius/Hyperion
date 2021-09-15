//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world_serializer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // NOTE: For now we don't really do any kind of validation.
    // That includes trusting the entities have the proper components and that there are no circular references.

    //--------------------------------------------------------------
    void WorldHierarchy::SetParent(EntityId entity, EntityId parent) {
        HierarchyComponent *entity_hierarchy = m_world->GetComponent<HierarchyComponent>(entity);
        HierarchyComponent *parent_hierarchy = m_world->GetComponent<HierarchyComponent>(parent);

        if (parent == Entity::EMPTY) {
            // TODO: Implement.

            m_roots.Add(entity);
        } else {
            if (entity_hierarchy->parent == Entity::EMPTY) {
                m_roots.Remove(entity);
            }

            // NOTE: This is kinda unintuitive, as we are inserting a child at the beginning and not at the end.

            entity_hierarchy->parent = parent;
            entity_hierarchy->next_sibling = parent_hierarchy->first_child;

            parent_hierarchy->child_count += 1;
            parent_hierarchy->first_child = entity;
        }
    }

}