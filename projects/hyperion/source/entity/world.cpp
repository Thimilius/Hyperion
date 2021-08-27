//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool World::IsValidId(EntityId id) {
        EntityIndex index = Entity::GetIndex(id);
        return index < m_entities.GetLength() && m_entities[index].id == id;
    }

    //--------------------------------------------------------------
    EntityGuid World::GetGuid(EntityId id) {
        if (IsValidId(id)) {
            return m_entities[Entity::GetIndex(id)].guid;
        } else {
            HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
            return EntityGuid();
        }
    }

    //--------------------------------------------------------------
    EntityId World::CreateEntity() {
        if (m_free_entity_indices.IsEmpty()) {
            m_entities.Add({ Entity::CreateId(static_cast<EntityIndex>(m_entities.GetLength()), 0), Entity::CreateGuid() });
            return m_entities.GetLast().id;
        } else {
            EntityIndex new_index = m_free_entity_indices.GetLast();
            m_free_entity_indices.RemoveLast();

            EntityId new_id = Entity::CreateId(new_index, Entity::GetVersion(m_entities[new_index].id));
            m_entities[new_index].id = new_id;

            return new_id;
        }
    }

    //--------------------------------------------------------------
    void World::DestroyEntity(EntityId id) {
        if (IsValidId(id)) {
            EntityId new_id = Entity::CreateId(EntityIndex(-1), Entity::GetVersion(id) + 1);
            EntityIndex index = Entity::GetIndex(id);

            EntityDescription &entity_description = m_entities[index];
            entity_description.id = new_id;
            m_free_entity_indices.Add(index);

            for (EntityPool &entity_pool : m_entity_pools) {
                entity_pool.RemoveComponent(id);
            }
        } else {
            HYP_LOG_WARN("Entity", "Trying to destroy nonexistent entity with id {}.", id);
        }
    }

}