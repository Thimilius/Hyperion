#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_pool.hpp"

//-------------------- Forward Declarations --------------------
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
            Iterator(EntityIndex index, World *world, ComponentPool *smallest_pool, bool all) : m_index(index), m_world(world), m_smallest_pool(smallest_pool), m_all(all) {}
        public:
            inline EntityId operator*() const { return m_all ? m_world->m_entities[m_index].id : m_smallest_pool->GetEntity(m_index); }
            inline bool operator==(const Iterator &other) const { return m_index == other.m_index || m_index == m_world->m_entities.GetLength(); }
            inline bool operator!=(const Iterator &other) const { return !(*this == other); }
            inline Iterator &operator++() {
                if (m_all) {
                    m_index++;
                } else {
                    do {
                        m_index++;
                    } while (ShouldSkip());
                }
                return *this;
            }
        private:
            bool ShouldSkip() {
                const ComponentId COMPONENT_IDS[] = { 0, ComponentRegistry::GetId<T>() ... };

                if (m_index >= m_smallest_pool->GetEntityCount()) {
                    return false;
                }

                EntityId id = m_smallest_pool->GetEntity(m_index);
                for (uint64 i = 1; i < (sizeof...(T) + 1); i++) {
                    ComponentPool &component_pool = m_world->m_component_pools[COMPONENT_IDS[i]];
                    if (&component_pool != m_smallest_pool) {
                        if (!component_pool.HasComponent(id)) {
                            return true;
                        }
                    }
                }
                return false;
            }
        private:
            EntityIndex m_index;

            World *m_world;
            ComponentPool *m_smallest_pool;
            bool m_all;
        };
    public:
        WorldView(World *world) : m_world(world) {
            const ComponentId COMPONENT_IDS[] = { 0, ComponentRegistry::GetId<T>() ... };

            m_all = sizeof...(T) == 0;

            uint64 smallest_entity_count = UINT64_MAX;
            for (uint64 i = 1; i < (sizeof...(T) + 1); i++) {
                ComponentPool &component_pool = m_world->m_component_pools[COMPONENT_IDS[i]];
                if (component_pool.GetEntityCount() < smallest_entity_count) {
                    m_smallest_pool = &component_pool;
                    smallest_entity_count = component_pool.GetEntityCount();
                }
            }
        }
    public:
        const Iterator begin() const {
            const ComponentId COMPONENT_IDS[] = { 0, ComponentRegistry::GetId<T>() ... };

            EntityIndex index = 0;
            if (!m_all) {
                while (index < m_smallest_pool->GetEntityCount()) {
                    EntityId id = m_smallest_pool->GetEntity(index);
                    for (uint64 i = 1; i < (sizeof...(T) + 1); i++) {
                        ComponentPool &component_pool = m_world->m_component_pools[COMPONENT_IDS[i]];
                        if (&component_pool != m_smallest_pool) {
                            if (!component_pool.HasComponent(id)) {
                                index++;
                                goto outer;
                            }
                        }
                    }
                    break;
                outer:;
                }
            }

            return Iterator(index, m_world, m_smallest_pool, m_all);
        }
        const Iterator end() const {
            EntityIndex index = m_all ? static_cast<EntityIndex>(m_world->m_entities.GetLength()) : static_cast<EntityIndex>(m_smallest_pool->GetEntityCount());
            return Iterator(index, m_world, m_smallest_pool, m_all);
        }
    private:
        World *m_world;
        ComponentPool *m_smallest_pool;
        bool m_all;
    };

}