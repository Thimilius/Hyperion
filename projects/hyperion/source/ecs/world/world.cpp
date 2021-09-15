//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World::World() {
        for (const ComponentInfo &component_info : ComponentRegistry::GetComponents()) {
            m_component_pools.Add(ComponentPool(component_info));
        }
    }

    //--------------------------------------------------------------
    bool8 World::IsValidId(EntityId id) const {
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
    EntityId World::CreateEntity(EntityPrimitive primitive, EntityGuid guid) {
        EntityId id;
        if (m_available <= 0) {
            m_entities.Add({ EntityUtilities::CreateId(static_cast<EntityIndex>(m_entities.GetLength()), 0), guid });
            id = m_entities.GetLast().id;
        } else {
            EntityIndex new_index = m_next;
            m_next = EntityUtilities::GetIndex(m_entities[new_index].id);
            m_available--;

            EntityId new_id = EntityUtilities::CreateId(new_index, EntityUtilities::GetVersion(m_entities[new_index].id));
            m_entities[new_index].id = new_id;
            m_entities[new_index].guid = guid;

            id = new_id;
        }

        for (ComponentPool &component_pool : m_component_pools) {
            component_pool.FitIntoPool(id);
        }

        AddComponentsForPrimitive(id, primitive);

        return id;
    }

    //--------------------------------------------------------------
    void World::DestroyEntity(EntityId id) {
        if (IsValidId(id)) {
            EntityId new_id = EntityUtilities::CreateId(m_next, EntityUtilities::GetVersion(id) + 1);
            EntityIndex index = EntityUtilities::GetIndex(id);

            m_next = index;
            m_available++;

            EntityDescription &entity_description = m_entities[index];
            entity_description.id = new_id;

            for (ComponentPool &component_pool : m_component_pools) {
                component_pool.RemoveComponent(id);
            }
        } else {
            HYP_LOG_WARN("Entity", "Trying to destroy nonexistent entity with id {}.", id);
        }
    }

    //--------------------------------------------------------------
    void World::AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive) {
        if (primitive != EntityPrimitive::Empty) {
            AddComponent<TagComponent>(id);
            AddComponent<TransformComponent>(id);
            AddComponent<LocalToWorldComponent>(id);
        }

        switch (primitive) {
            case EntityPrimitive::Empty: break;
            case EntityPrimitive::Base: break;
            case EntityPrimitive::Camera: AddComponent<CameraComponent>(id); break;
            case EntityPrimitive::Sprite: AddComponent<SpriteComponent>(id); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

}