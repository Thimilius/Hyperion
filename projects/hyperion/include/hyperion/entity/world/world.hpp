#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/component/component.hpp"
#include "hyperion/entity/world/world_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class World final {
    public:
        bool IsValidId(EntityId id) const;
        EntityGuid GetGuid(EntityId id) const;

        EntityId CreateEntity();
        void DestroyEntity(EntityId id);

        template<typename T>
        void RegisterComponent() {
            ComponentId component_id = Component::GetId<T>();
            ComponentDestructorFunction component_destructor = [](const void *component) { static_cast<const T *>(component)->~T(); };
            m_entity_pools.Add(EntityPool(MAX_ENTITIES, component_id, sizeof(T), component_destructor));
        }

        template<typename T>
        T *AddComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                EntityPool &entity_pool = m_entity_pools[component_id];

                byte *component_data = entity_pool.AddComponent(id);
                if (component_data != nullptr) {
                    T *component = new(component_data) T();
                    return component;
                } else {
                    HYP_LOG_WARN("Entity", "Trying to add already existent component type to entity with id {}.", id);
                    return nullptr;
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to add component to nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        bool HasComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                EntityPool &entity_pool = m_entity_pools[component_id];
                return entity_pool.HasComponent(id);
            } else {
                HYP_LOG_WARN("Entity", "Trying to check component type of nonexistent entity with id {}.", id);
                return false;
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
                if (!entity_pool.RemoveComponent(id)) {
                    HYP_LOG_WARN("Entity", "Trying to remove nonexistent component type from entity whith id {}.", id);
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to remove component from nonexistent entity with id {}.", id);
            }
        }

        template<typename... T>
        WorldView<T ...> GetView() {
            return WorldView<T ...>(this);
        }
    private:
        List<EntityDescription> m_entities;
        List<EntityIndex> m_free_entity_indices;

        List<EntityPool> m_entity_pools;
    private:
        inline static const uint32 MAX_ENTITIES = 4096;
    private:
        template<typename... T>
        friend class Hyperion::WorldView;
    };

}