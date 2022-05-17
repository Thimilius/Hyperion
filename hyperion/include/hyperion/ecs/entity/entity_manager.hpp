//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"
#include "hyperion/ecs/component/component_pool.hpp"
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/entity/entity_archetype.hpp"
#include "hyperion/ecs/entity/entity_hierarchy.hpp"
#include "hyperion/ecs/entity/entity_utilities.hpp"
#include "hyperion/ecs/entity/entity_view.hpp"
#include "hyperion/ecs/world/world_environment.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  struct HierarchyComponent;
  class Mesh;
  class World;
  class WorldSerializer;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct EntityManagerStorage {
    Array<EntityDescription> entities;
    Map<EntityGuid, EntityId> entities_by_guid;
    uint64 available = 0;
    EntityIndex next = EntityUtilities::GetIndex(Entity::EMPTY);

    Array<ComponentPool> component_pools;
    Array<ComponentCallbacks> component_callbacks;
  };

  class EntityManager final {
  public:
    World *GetWorld() const { return m_world; }
    
    bool8 IsAlive(EntityId id) const;

    EntityGuid GetGuid(EntityId id) const;
    EntityId GetByGuid(EntityGuid guid) const;

    EntityId CreateEntity(EntityPrimitive primitive = EntityPrimitive::Base, EntityGuid guid = EntityGuid::Generate());
    EntityId CreateMultiMeshEntity(Mesh *mesh);
    void DestroyEntity(EntityId id, EntityHierarchyDestructionPolicy hierarchy_destruction_policy = EntityHierarchyDestructionPolicy::DestroyChildren);

    EntityArchetype *CreateArchetype(EntityId id);
    EntityId Instantiate(EntityArchetype *archetype);

    void SetEnabled(EntityId entity, bool8 enabled);
    
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

    void *AddComponent(ComponentId component_id, EntityId id) {
      HYP_PROFILE_SCOPE("World.AddComponent");

      if (IsAlive(id)) {
        ComponentPool &component_pool = m_storage.component_pools[component_id];

        byte *component_data = component_pool.AddComponent(id);
        if (component_data != nullptr) {
          void *component = component_pool.GetComponentInfo().constructor(component_data);
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

    bool8 HasComponent(ComponentId component_id, EntityId id) {
      HYP_PROFILE_SCOPE("World.HasComponent");

      if (IsAlive(id)) {
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

    void *GetComponent(ComponentId component_id, EntityId id) {
      HYP_PROFILE_SCOPE("World.GetComponent");

      if (IsAlive(id)) {
        ComponentPool &component_pool = m_storage.component_pools[component_id];
        return component_pool.GetComponent(id);
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

    void RemoveComponent(ComponentId component_id, EntityId id) {
      HYP_PROFILE_SCOPE("World.RemoveComponent");

      if (IsAlive(id)) {
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
    EntityView<GetComponents<Component ...>, ExcludeComponents<DisabledComponent, Exclude ...>> GetView(ExcludeComponents<Exclude ...>  = { }) {
      return EntityView<GetComponents<Component ...>, ExcludeComponents<DisabledComponent, Exclude ...>>(this);
    }

    template<typename... Component, typename... Exclude>
    EntityView<GetComponents<Component ...>, ExcludeComponents<Exclude ...>> GetViewAll(ExcludeComponents<Exclude ...>  = { }) {
      return EntityView<GetComponents<Component ...>, ExcludeComponents<Exclude ...>>(this);
    }
    
    inline void RegisterOnEntityCreated(const EntityCallback &callback) {
      m_entity_created_callbacks.Add(callback);
    }

    inline void UnregisterOnEntityCreated(const EntityCallback &callback) {
      m_entity_created_callbacks.Remove(callback);
    }

    inline void RegisterOnEntityDestroyed(const EntityCallback &callback) {
      m_entity_destroyed_callbacks.Add(callback);
    }

    inline void UnregisterOnEntityDestroyed(const EntityCallback &callback) {
      m_entity_destroyed_callbacks.Remove(callback);
    }

    template<typename T>
    inline void RegisterOnComponentAdded(const ComponentCallback &callback) {
      ComponentId component_id = ComponentRegistry::GetId<T>();
      m_storage.component_callbacks[component_id].added.Add(callback);
    }

    template<typename T>
    inline void UnregisterOnComponentAdded(const ComponentCallback &callback) {
      ComponentId component_id = ComponentRegistry::GetId<T>();
      m_storage.component_callbacks[component_id].added.Remove(callback);
    }

    template<typename T>
    inline void RegisterOnComponentRemoved(const ComponentCallback &callback) {
      ComponentId component_id = ComponentRegistry::GetId<T>();
      m_storage.component_callbacks[component_id].removed.Add(callback);
    }

    template<typename T>
    inline void UnregisterOnComponentRemoved(const ComponentCallback &callback) {
      ComponentId component_id = ComponentRegistry::GetId<T>();
      m_storage.component_callbacks[component_id].removed.Remove(callback);
    }
  private:
    EntityManager();
  private:
    void AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive);
  private: 
    static String GetNameForPrimitive(EntityPrimitive primitive);
  private:
    World *m_world = nullptr;
    
    EntityManagerStorage m_storage;

    Array<EntityCallback> m_entity_created_callbacks;
    Array<EntityCallback> m_entity_destroyed_callbacks;
  private:
    friend class Hyperion::World;
    template<typename... Types>
    friend class Hyperion::EntityView;
  };
  
}
