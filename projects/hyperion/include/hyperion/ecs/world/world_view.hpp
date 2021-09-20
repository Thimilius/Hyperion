#pragma once

#include <array>

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_pool.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    template<typename... Types>
    struct ComponentTypesList { };

    template<typename... Types>
    struct GetComponents : public ComponentTypesList<Types ...> { };

    template<typename... Types>
    struct ExcludeComponents : public ComponentTypesList<Types ...> { };

    template<typename... Types> class WorldView;

    template<typename... Component, typename... Exclude>
    class WorldView<GetComponents<Component ...>, ExcludeComponents<Exclude ...>> {
    public:
        class Iterator {
        public:
            Iterator(EntityIndex index, World *world, ComponentPool *smallest_pool, bool8 all) : m_index(index), m_world(world), m_smallest_pool(smallest_pool), m_all(all) { }
        public:
            inline EntityId operator*() const { return m_all ? m_world->m_storage.entities[m_index].id : m_smallest_pool->GetEntity(m_index); }
            inline bool8 operator==(const Iterator &other) const { return m_index == other.m_index || m_index == m_world->m_storage.entities.GetLength(); }
            inline bool8 operator!=(const Iterator &other) const { return !(*this == other); }
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
            bool8 ShouldSkip() {
                if (m_index >= m_smallest_pool->GetEntityCount()) {
                    return false;
                }

                EntityId id = m_smallest_pool->GetEntity(m_index);

                // First check that the entity has all the components we want.
                for (uint64 i = 0; i < COMPONENT_IDS.size(); i++) {
                    ComponentPool &component_pool = m_world->m_storage.component_pools[COMPONENT_IDS[i]];
                    if (&component_pool != m_smallest_pool) {
                        if (!component_pool.HasComponent(id)) {
                            return true;
                        }
                    }
                }

                // Now check that the entity does not have the components we want to exclude.
                for (uint64 i = 0; i < EXCLUDE_IDS.size(); i++) {
                    ComponentPool &component_pool = m_world->m_storage.component_pools[EXCLUDE_IDS[i]];
                    if (&component_pool != m_smallest_pool) {
                        if (component_pool.HasComponent(id)) {
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
            bool8 m_all;

            const std::array<ComponentId, (sizeof...(Component))> COMPONENT_IDS = { ComponentRegistry::GetId<Component>() ... };
            const std::array<ComponentId, (sizeof...(Exclude))> EXCLUDE_IDS = { ComponentRegistry::GetId<Exclude>() ... };
        };
    public:
        WorldView(World *world) : m_world(world) {
            m_all = (sizeof...(Component)) == 0;

            uint64 smallest_entity_count = UINT64_MAX;
            for (uint64 i = 0; i < COMPONENT_IDS.size(); i++) {
                ComponentPool &component_pool = m_world->m_storage.component_pools[COMPONENT_IDS[i]];
                if (component_pool.GetEntityCount() < smallest_entity_count) {
                    m_smallest_pool = &component_pool;
                    smallest_entity_count = component_pool.GetEntityCount();
                }
            }
        }
    public:
        const Iterator begin() const {
            EntityIndex index = 0;
            if (!m_all) {
                while (index < m_smallest_pool->GetEntityCount()) {
                    EntityId id = m_smallest_pool->GetEntity(index);

                    // First check that the entity has all the components we want.
                    for (uint64 i = 0; i < COMPONENT_IDS.size(); i++) {
                        ComponentPool &component_pool = m_world->m_storage.component_pools[COMPONENT_IDS[i]];
                        if (&component_pool != m_smallest_pool) {
                            if (!component_pool.HasComponent(id)) {
                                index++;
                                goto L_CHECK_NEXT_ENTITY;
                            }
                        }
                    }

                    // Now check that the entity does not have the components we want to exclude.
                    for (uint64 i = 0; i < EXCLUDE_IDS.size(); i++) {
                        ComponentPool &component_pool = m_world->m_storage.component_pools[EXCLUDE_IDS[i]];
                        if (&component_pool != m_smallest_pool) {
                            if (component_pool.HasComponent(id)) {
                                index++;
                                goto L_CHECK_NEXT_ENTITY;
                            }
                        }
                    }

                    break;
                L_CHECK_NEXT_ENTITY:;
                }
            }

            return Iterator(index, m_world, m_smallest_pool, m_all);
        }
        const Iterator end() const {
            EntityIndex index = m_all ? static_cast<EntityIndex>(m_world->m_storage.entities.GetLength()) : static_cast<EntityIndex>(m_smallest_pool->GetEntityCount());
            return Iterator(index, m_world, m_smallest_pool, m_all);
        }
    private:
        World *m_world;
        ComponentPool *m_smallest_pool;
        bool8 m_all;
    private:
        const std::array<ComponentId, (sizeof...(Component))> COMPONENT_IDS = { ComponentRegistry::GetId<Component>() ... };
        const std::array<ComponentId, (sizeof...(Exclude))> EXCLUDE_IDS = { ComponentRegistry::GetId<Exclude>() ... };
    };

}