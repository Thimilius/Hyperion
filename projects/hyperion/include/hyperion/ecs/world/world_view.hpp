//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
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
    private:
        inline static constexpr uint64 COMPONENT_IDS_LENGTH = (sizeof...(Component));
        inline static constexpr uint64 EXCLUDE_IDS_LENGTH = (sizeof...(Exclude));

        inline static constexpr bool8 ALL_COMPONENTS = (sizeof...(Component)) == 0;

        static_assert((COMPONENT_IDS_LENGTH > 0) || (ALL_COMPONENTS && EXCLUDE_IDS_LENGTH == 0), "Exclude only is not supported");
    public:
        class Iterator {
        public:
            Iterator(EntityIndex index, World *world, ComponentPool *smallest_pool) : m_index(index), m_world(world), m_smallest_pool(smallest_pool) { }
        public:
            inline EntityId operator*() const { 
                if constexpr (ALL_COMPONENTS) {
                    return m_world->m_storage.entities[m_index].id;
                } else {
                    return m_smallest_pool->GetEntity(m_index);
                }
            }
            
            inline Iterator &operator++() {
                if constexpr (ALL_COMPONENTS) {
                    m_index++;
                } else {
                    do {
                        m_index++;
                    } while (ShouldSkip());
                }
                return *this;
            }

            inline bool8 operator==(const Iterator &other) const { return m_index == other.m_index || m_index == m_world->m_storage.entities.GetLength(); }
            inline bool8 operator!=(const Iterator &other) const { return !(*this == other); }
        private:
            bool8 ShouldSkip() {
                if (m_index >= m_smallest_pool->GetEntityCount()) {
                    return false;
                }

                EntityId id = m_smallest_pool->GetEntity(m_index);

                // First check that the entity has all the components we want.
                for (uint64 i = 0; i < COMPONENT_IDS_LENGTH; i++) {
                    ComponentPool &component_pool = m_world->m_storage.component_pools[COMPONENT_IDS[i]];
                    if (&component_pool != m_smallest_pool) {
                        if (!component_pool.HasComponent(id)) {
                            return true;
                        }
                    }
                }

                // Now check that the entity does not have the components we want to exclude.
                if constexpr (EXCLUDE_IDS_LENGTH > 0) {
                    for (uint64 i = 0; i < EXCLUDE_IDS_LENGTH; i++) {
                        ComponentPool &component_pool = m_world->m_storage.component_pools[EXCLUDE_IDS[i]];
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

            const std::array<ComponentId, COMPONENT_IDS_LENGTH> COMPONENT_IDS = { ComponentRegistry::GetId<Component>() ... };
            const std::array<ComponentId, EXCLUDE_IDS_LENGTH> EXCLUDE_IDS = { ComponentRegistry::GetId<Exclude>() ... };
        };
    public:
        WorldView(World *world) : m_world(world) {
            uint64 smallest_entity_count = UINT64_MAX;
            for (uint64 i = 0; i < COMPONENT_IDS_LENGTH; i++) {
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
            if constexpr (!ALL_COMPONENTS) {
                while (index < m_smallest_pool->GetEntityCount()) {
                    EntityId id = m_smallest_pool->GetEntity(index);

                    // First check that the entity has all the components we want.
                    for (uint64 i = 0; i < COMPONENT_IDS_LENGTH; i++) {
                        ComponentPool &component_pool = m_world->m_storage.component_pools[COMPONENT_IDS[i]];
                        if (&component_pool != m_smallest_pool) {
                            if (!component_pool.HasComponent(id)) {
                                index++;
                                goto L_CHECK_NEXT_ENTITY;
                            }
                        }
                    }

                    // Now check that the entity does not have the components we want to exclude.
                    if constexpr (EXCLUDE_IDS_LENGTH > 0) {
                        for (uint64 i = 0; i < EXCLUDE_IDS_LENGTH; i++) {
                            ComponentPool &component_pool = m_world->m_storage.component_pools[EXCLUDE_IDS[i]];
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

            return Iterator(index, m_world, m_smallest_pool);
        }
        const Iterator end() const {
            EntityIndex index;
            if constexpr (ALL_COMPONENTS) {
                index = static_cast<EntityIndex>(m_world->m_storage.entities.GetLength());
            } else {
                index = static_cast<EntityIndex>(m_smallest_pool->GetEntityCount());
            }
            return Iterator(index, m_world, m_smallest_pool);
        }
    private:
        World *m_world;
        ComponentPool *m_smallest_pool;

        // NOTE: The id arrays could probably be constexpr too.
        const std::array<ComponentId, COMPONENT_IDS_LENGTH> COMPONENT_IDS = { ComponentRegistry::GetId<Component>() ... };
        const std::array<ComponentId, EXCLUDE_IDS_LENGTH> EXCLUDE_IDS = { ComponentRegistry::GetId<Exclude>() ... };
    };

}