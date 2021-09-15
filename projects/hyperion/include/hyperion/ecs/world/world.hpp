#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_pool.hpp"
#include "hyperion/ecs/entity/entity_utilities.hpp"
#include "hyperion/ecs/world/world_view.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class World final {
    private:
        World();
        ~World() = default;
    public:
        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        bool8 IsValidId(EntityId id) const;
        EntityGuid GetGuid(EntityId id) const;

        EntityId CreateEntity(EntityPrimitive primitive = EntityPrimitive::Base, EntityGuid guid = EntityGuid::Generate());
        void DestroyEntity(EntityId id);

        template<typename T>
        T *AddComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
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
        bool8 HasComponent(EntityId id) {
            if (IsValidId(id)) {
                ComponentId component_id = ComponentRegistry::GetId<T>();
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
                ComponentId component_id = ComponentRegistry::GetId<T>();
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
                ComponentId component_id = ComponentRegistry::GetId<T>();
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
        void AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive);
    private:
        String m_name = "World";

        Array<EntityDescription> m_entities;
        uint64 m_available = 0;
        EntityIndex m_next = EntityUtilities::GetIndex(Entity::EMPTY);
        
        Array<ComponentPool> m_component_pools;
    private:
    private:
        friend class Hyperion::WorldManager;
        template<typename... T>
        friend class Hyperion::WorldView;
    };

}