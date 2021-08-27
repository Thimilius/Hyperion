#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/component.hpp"
#include "hyperion/entity/entity.hpp"

namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    template<typename... T>
    class WorldView {
    public:
        class Iterator {
        public:
            Iterator(EntityIndex index, World *world, EntityPool *smallest_pool, bool all) : m_index(index), m_world(world), m_smallest_pool(smallest_pool), m_all(all) { }
        public:
            inline EntityId operator*() const { return m_all ? m_world->m_entities[m_index].id : m_smallest_pool->GetEntity(m_index); }
            inline bool operator==(const Iterator &other) const { return m_index == other.m_index || m_index == m_world->m_entities.GetLength(); }
            inline bool operator!=(const Iterator &other) const { return !(*this == other); }
            inline Iterator &operator++() { m_index++; return *this; }
        private:
            EntityIndex m_index;

            World *m_world;
            EntityPool *m_smallest_pool;
            bool m_all;
        };
    public:
        WorldView(World *world) : m_world(world) {
            m_all = sizeof...(T) == 0;

            ComponentId components[] = { 0, Component::GetId<T>() ... };
            uint64 smallest_entity_count = UINT64_MAX;
            for (uint64 i = 1; i < (sizeof...(T) + 1); i++) {
                EntityPool &entity_pool = m_world->m_entity_pools[components[i]];
                if (entity_pool.GetEntityCount() < smallest_entity_count) {
                    m_smallest_pool = &entity_pool;
                }
            }
        }
    public:
        const Iterator begin() const { return Iterator(0, m_world, m_smallest_pool, m_all); }
        const Iterator end() const {
            EntityIndex index = m_all ? static_cast<EntityIndex>(m_world->m_entities.GetLength()) : static_cast<EntityIndex>(m_smallest_pool->GetEntityCount());
            return Iterator(index, m_world, m_smallest_pool, m_all);
        }
    private:
        World *m_world;
        EntityPool *m_smallest_pool;
        bool m_all;
    };

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