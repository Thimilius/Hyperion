//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool World::IsValidId(EntityId id) const {
        EntityIndex index = EntityUtilities::GetIndex(id);
        return index < m_entities.GetLength() && m_entities[index].id == id;
    }

    //--------------------------------------------------------------
    EntityGuid World::GetGuid(EntityId id) const {
        if (IsValidId(id)) {
            return m_entities[EntityUtilities::GetIndex(id)].guid;
        } else {
            HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
            return EntityGuid();
        }
    }

    //--------------------------------------------------------------
    EntityId World::CreateEntity() {
        if (m_free_entity_indices.IsEmpty()) {
            m_entities.Add({ EntityUtilities::CreateId(static_cast<EntityIndex>(m_entities.GetLength()), 0), EntityUtilities::CreateGuid() });
            return m_entities.GetLast().id;
        } else {
            EntityIndex new_index = m_free_entity_indices.GetLast();
            m_free_entity_indices.RemoveLast();

            EntityId new_id = EntityUtilities::CreateId(new_index, EntityUtilities::GetVersion(m_entities[new_index].id));
            m_entities[new_index].id = new_id;

            return new_id;
        }
    }

    //--------------------------------------------------------------
    void World::DestroyEntity(EntityId id) {
        if (IsValidId(id)) {
            EntityId new_id = EntityUtilities::CreateId(EntityIndex(-1), EntityUtilities::GetVersion(id) + 1);
            EntityIndex index = EntityUtilities::GetIndex(id);

            EntityDescription &entity_description = m_entities[index];
            entity_description.id = new_id;
            m_free_entity_indices.Add(index);

            for (ComponentPool &component_pool : m_component_pools) {
                component_pool.RemoveComponent(id);
            }
        } else {
            HYP_LOG_WARN("Entity", "Trying to destroy nonexistent entity with id {}.", id);
        }
    }

}