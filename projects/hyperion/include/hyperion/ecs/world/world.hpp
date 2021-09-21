#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"
#include "hyperion/ecs/component/component_pool.hpp"
#include "hyperion/ecs/entity/entity_utilities.hpp"
#include "hyperion/ecs/world/world_environment.hpp"
#include "hyperion/ecs/world/world_hierarchy.hpp"
#include "hyperion/ecs/world/world_view.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct WorldStorage {
        Array<EntityDescription> entities;
        Map<EntityGuid, EntityId> entities_by_guid;
        uint64 available = 0;
        EntityIndex next = EntityUtilities::GetIndex(Entity::EMPTY);

        Array<ComponentPool> component_pools;
        Array<ComponentCallbacks> component_callbacks;
    };

    class World final {
    private:
        World();
        ~World() = default;
    public:
        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }
        inline WorldEnvironment &GetEnvironment() { return m_environment; }
        inline const WorldEnvironment &GetEnvironment() const { return m_environment; }

        inline WorldHierarchy *GetHierarchy() { return &m_hierarchy; }
        inline const WorldHierarchy *GetHierarchy() const { return &m_hierarchy; }

        inline Physics::IPhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }

        bool8 IsAlive(EntityId id) const;

        EntityGuid GetGuid(EntityId id) const;
        EntityId GetByGuid(EntityGuid guid) const;

        EntityId CreateEntity(EntityPrimitive primitive = EntityPrimitive::Base, EntityGuid guid = EntityGuid::Generate());
        void DestroyEntity(EntityId id, WorldHierarchyDestructionPolicy hierarchy_destruction_policy = WorldHierarchyDestructionPolicy::DestroyChildren);

        template<typename T>
        T *AddComponent(EntityId id) {
            HYP_PROFILE_SCOPE("World.AddComponent");

            if (IsAlive(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
                ComponentPool &component_pool = m_storage.component_pools[component_id];

                byte *component_data = component_pool.AddComponent(id);
                if (component_data != nullptr) {
                    T *component = new(component_data) T();
                    for (ComponentCallback callback : m_storage.component_callbacks[component_id].added) {
                        callback(this, id);
                    }
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
        bool8 HasComponent(EntityId id) {
            HYP_PROFILE_SCOPE("World.HasComponent");

            if (IsAlive(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
                ComponentPool &component_pool = m_storage.component_pools[component_id];
                return component_pool.HasComponent(id);
            } else {
                HYP_LOG_WARN("Entity", "Trying to check component type of nonexistent entity with id {}.", id);
                return false;
            }
        }

        template<typename T>
        T *GetComponent(EntityId id) {
            HYP_PROFILE_SCOPE("World.GetComponent");

            if (IsAlive(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
                ComponentPool &component_pool = m_storage.component_pools[component_id];
                byte *component_data = component_pool.GetComponent(id);
                return reinterpret_cast<T *>(component_data);
            } else {
                HYP_LOG_WARN("Entity", "Trying to get component from nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        T *GetOrAddComponent(EntityId id) {
            HYP_PROFILE_SCOPE("World.GetOrAddComponent");

            if (IsAlive(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
                ComponentPool &component_pool = m_storage.component_pools[component_id];
                if (component_pool.HasComponent(id)) {
                    byte *component_data = component_pool.GetComponent(id);
                    return reinterpret_cast<T *>(component_data);
                } else {
                    byte *component_data = component_pool.AddComponent(id);
                    if (component_data != nullptr) {
                        T *component = new(component_data) T();
                        for (ComponentCallback callback : m_storage.component_callbacks[component_id].added) {
                            callback(this, id);
                        }
                        return component;
                    } else {
                        HYP_LOG_WARN("Entity", "Trying to add already existent component type to entity with id {}.", id);
                        return nullptr;
                    }
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to get or add component from nonexistent entity with id {}.", id);
                return nullptr;
            }
        }

        template<typename T>
        void RemoveComponent(EntityId id) {
            HYP_PROFILE_SCOPE("World.RemoveComponent");

            if (IsAlive(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
                ComponentPool &component_pool = m_storage.component_pools[component_id];
                if (component_pool.HasComponent(id)) {
                    for (ComponentCallback callback : m_storage.component_callbacks[component_id].removed) {
                        callback(this, id);
                    }
                    component_pool.RemoveComponent(id);
                } else {
                    HYP_LOG_WARN("Entity", "Trying to remove nonexistent component type from entity whith id {}.", id);
                }
            } else {
                HYP_LOG_WARN("Entity", "Trying to remove component from nonexistent entity with id {}.", id);
            }
        }

        template<typename... Component, typename... Exclude>
        WorldView<GetComponents<Component ...>, ExcludeComponents<Exclude ...>> GetView(ExcludeComponents<Exclude ...> = {}) {
            return WorldView<GetComponents<Component ...>, ExcludeComponents<Exclude ...>>(this);
        }

        template<typename T>
        void OnComponentAdded(const ComponentCallback &callback) {
            ComponentId component_id = ComponentRegistry::GetId<T>();
            m_storage.component_callbacks[component_id].added.Add(callback);
        }

        template<typename T>
        void OnComponentRemoved(const ComponentCallback &callback) {
            ComponentId component_id = ComponentRegistry::GetId<T>();
            m_storage.component_callbacks[component_id].removed.Add(callback);
        }
    private:
        void AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive);
    private:
        String m_name = "World";
        WorldEnvironment m_environment;

        WorldHierarchy m_hierarchy;
        WorldStorage m_storage;

        Physics::IPhysicsWorld *m_physics_world;
    private:
    private:
        friend class Hyperion::WorldManager;
        template<typename... Types>
        friend class Hyperion::WorldView;
    };

}