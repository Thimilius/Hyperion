#pragma once

#include "hyperion/entity/component.hpp"
#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    class World {
    public:
        EntityGuid GetGuid(EntityId id);

        EntityId CreateEntity();
        void DestroyEntity(EntityId id);

        bool IsValidId(EntityId id);

        template<typename T>
        T *AddComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();

                if (m_component_pools.GetLength() <= component_id) {
                    m_component_pools.Resize(component_id + 1);
                }
                if (m_component_pools[component_id] == nullptr) {
                    m_component_pools[component_id].reset(new DataPool(sizeof(T), MAX_ENTITIES));
                }

                T *component = new (m_component_pools[component_id]->Get(id)) T();
                m_entities[Entity::GetIndex(id)].component_mask.set(component_id);
                return component;
            } else {
                HYP_LOG_WARN("Entity", "Trying to add component to nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        T *Get(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                
                if (m_entities[id].component_mask.test(component_id)) {
                    T *component = static_cast<T *>(m_component_pools[component_id]->Get(Entity::GetIndex(id)));
                    return component;
                } else {
                    return nullptr;
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to get component from nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        void RemoveComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                m_entities[Entity::GetIndex(id)].component_mask.reset(component_id);
            } else {
                HYP_LOG_WARN("Entity", "Trying to remove component from nonexistent entity with id {}.", id);
            }
        }
    private:
        const uint32 MAX_ENTITIES = 4069;
    private:
        List<EntityDescription> m_entities;
        List<std::unique_ptr<DataPool>> m_component_pools;

        List<EntityIndex> m_free_entity_indices;
    };

}