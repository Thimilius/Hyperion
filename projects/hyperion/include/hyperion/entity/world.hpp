#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/component.hpp"
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class World final {
    public:
        bool IsValidId(EntityId id);
        EntityGuid GetGuid(EntityId id);

        EntityId CreateEntity();
        void DestroyEntity(EntityId id);

        template<typename T>
        void RegisterComponent() {
            ComponentId component_id = Component::GetId<T>();
            m_entity_pools.Add(EntityPool(MAX_ENTITIES, component_id, sizeof(T)));
        }

        template<typename T>
        T *AddComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                EntityPool &entity_pool = m_entity_pools[component_id];

                byte *component_data = entity_pool.AddComponent(id);
                T *component = new(component_data) T();
                return component;
            } else {
                HYP_LOG_WARN("Entity", "Trying to add component to nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        T *GetComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                EntityPool &entity_pool = m_entity_pools[component_id];

                byte *component_data = entity_pool.GetComponent(id);
                return reinterpret_cast<T *>(component_data);
            } else {
                HYP_LOG_WARN("Entity", "Trying to get component from nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        void RemoveComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                EntityPool &entity_pool = m_entity_pools[component_id];

                byte *component_data = entity_pool.GetComponent(id);
                if (component_data != nullptr) {
                    T *component = reinterpret_cast<T *>(component_data);
                    component->~T();

                    entity_pool.RemoveComponent(id);
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to remove component from nonexistent entity with id {}.", id);
            }
        }
    private:
        List<EntityDescription> m_entities;
        List<EntityIndex> m_free_entity_indices;

        List<EntityPool> m_entity_pools;
    private:
        inline static const uint32 MAX_ENTITIES = 4096;
    };

}