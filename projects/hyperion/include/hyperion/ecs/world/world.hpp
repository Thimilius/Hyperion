#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_pool.hpp"
#include "hyperion/ecs/world/world_view.hpp"

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
            ComponentInfo component_info;
            component_info.id = Component::GetId<T>();
            component_info.element_size = sizeof(T);
            component_info.destructor = [](const void *component) { static_cast<const T *>(component)->~T(); };
            m_component_pools.Add(ComponentPool(MAX_ENTITIES, component_info));
        }

        template<typename T>
        T *AddComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                ComponentPool &component_pool = m_component_pools[component_id];

                byte *component_data = component_pool.AddComponent(id);
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
                ComponentPool &component_pool = m_component_pools[component_id];
                return component_pool.HasComponent(id);
            } else {
                HYP_LOG_WARN("Entity", "Trying to check component type of nonexistent entity with id {}.", id);
                return false;
            }
        }

        template<typename T>
        T *GetComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = Component::GetId<T>();
                ComponentPool &component_pool = m_component_pools[component_id];
                byte *component_data = component_pool.GetComponent(id);
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
                ComponentPool &component_pool = m_component_pools[component_id];
                if (!component_pool.RemoveComponent(id)) {
                    HYP_LOG_WARN("Entity", "Trying to remove nonexistent component type from entity whith id {}.", id);
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to remove component from nonexistent entity with id {}.", id);
            }
        }

        template<typename... T>
        WorldView<T ...> GetView() { return WorldView<T ...>(this); }
    private:
        List<EntityDescription> m_entities;
        // TODO: Remove free entities by using an implicit list as described here: https://skypjack.github.io/2019-05-06-ecs-baf-part-3/.
        List<EntityIndex> m_free_entity_indices;

        List<ComponentPool> m_component_pools;
    private:
        inline static const uint32 MAX_ENTITIES = 4096;
    private:
        template<typename... T>
        friend class Hyperion::WorldView;
    };

}