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
        m_hierarchy.m_world = this;

        for (const ComponentInfo &component_info : ComponentRegistry::GetComponents()) {
            m_storage.component_pools.Add(ComponentPool(component_info));
        }
    }

    //--------------------------------------------------------------
    bool8 World::IsValidId(EntityId id) const {
        EntityIndex index = EntityUtilities::GetIndex(id);
        return index < m_storage.entities.GetLength() && m_storage.entities[index].id == id;
    }

    //--------------------------------------------------------------
    EntityGuid World::GetGuid(EntityId id) const {
        if (IsValidId(id)) {
            return m_storage.entities[EntityUtilities::GetIndex(id)].guid;
        } else {
            HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
            return EntityGuid();
        }
    }

    //--------------------------------------------------------------
    EntityId World::CreateEntity(EntityPrimitive primitive, EntityGuid guid) {
        EntityId id;
        if (m_storage.available <= 0) {
            m_storage.entities.Add({ EntityUtilities::CreateId(static_cast<EntityIndex>(m_storage.entities.GetLength()), 0), guid });
            id = m_storage.entities.GetLast().id;
        } else {
            EntityIndex new_index = m_storage.next;
            m_storage.next = EntityUtilities::GetIndex(m_storage.entities[new_index].id);
            m_storage.available--;

            EntityId new_id = EntityUtilities::CreateId(new_index, EntityUtilities::GetVersion(m_storage.entities[new_index].id));
            m_storage.entities[new_index].id = new_id;
            m_storage.entities[new_index].guid = guid;

            id = new_id;
        }

        for (ComponentPool &component_pool : m_storage.component_pools) {
            component_pool.FitIntoPool(id);
        }

        AddComponentsForPrimitive(id, primitive);

        return id;
    }

    //--------------------------------------------------------------
    void World::DestroyEntity(EntityId id) {
        if (IsValidId(id)) {
            {
                m_hierarchy.m_roots.Remove(id);
            }

            EntityId new_id = EntityUtilities::CreateId(m_storage.next, EntityUtilities::GetVersion(id) + 1);
            EntityIndex index = EntityUtilities::GetIndex(id);

            m_storage.next = index;
            m_storage.available++;

            EntityDescription &entity_description = m_storage.entities[index];
            entity_description.id = new_id;

            for (ComponentPool &component_pool : m_storage.component_pools) {
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
            AddComponent<LocalTransformComponent>(id);
            AddComponent<DerivedTransformComponent>(id);
            AddComponent<LocalToWorldComponent>(id);
            AddComponent<HierarchyComponent>(id);

            {
                m_hierarchy.m_roots.Add(id);
            }
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