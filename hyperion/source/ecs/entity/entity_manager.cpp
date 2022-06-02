//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/entity/entity_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {
  
  //--------------------------------------------------------------
  EntityManager::EntityManager() {
    uint64 component_count = ComponentRegistry::GetComponentInfos().GetLength();
    m_storage.component_pools.Reserve(component_count);
    for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
      m_storage.component_pools.Add(ComponentPool(component_info));
    }
    m_storage.component_callbacks.Resize(component_count);
  }
  
  //--------------------------------------------------------------
  bool8 EntityManager::IsAlive(EntityId id) const {
    EntityIndex index = EntityId::GetIndex(id);
    return index < m_storage.entities.GetLength() && m_storage.entities[index].id == id;
  }

  //--------------------------------------------------------------
  EntityGuid EntityManager::GetGuid(EntityId id) const {
    HYP_PROFILE_SCOPE("World.GetGuid");

    if (IsAlive(id)) {
      return m_storage.entities[EntityId::GetIndex(id)].guid;
    } else {
      HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
      return EntityGuid();
    }
  }

  //--------------------------------------------------------------
  EntityId EntityManager::GetByGuid(EntityGuid guid) const {
    HYP_PROFILE_SCOPE("World.GetByGuid");

    auto it = m_storage.entities_by_guid.Find(guid);
    if (it == m_storage.entities_by_guid.end()) {
      HYP_LOG_WARN("Entity", "Trying to get id from nonexistent entity with guid {}.", guid.ToString());
      return EntityId::EMPTY;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  EntityId EntityManager::CreateEntity(EntityPrimitive primitive, EntityGuid guid) {
    HYP_PROFILE_SCOPE("World.CreateEntity");

    EntityId id;
    if (m_storage.available <= 0) {
      m_storage.entities.Add({ EntityId::Create(static_cast<EntityIndex>(m_storage.entities.GetLength()), 0), guid });
      id = m_storage.entities.GetLast().id;
    } else {
      EntityIndex new_index = m_storage.next;
      m_storage.next = EntityId::GetIndex(m_storage.entities[new_index].id);
      m_storage.available--;

      EntityId new_id = EntityId::Create(new_index, EntityId::GetVersion(m_storage.entities[new_index].id));
      m_storage.entities[new_index].id = new_id;
      m_storage.entities[new_index].guid = guid;

      id = new_id;
    }
    m_storage.entities_by_guid.Insert(guid, id);

    for (ComponentPool &component_pool : m_storage.component_pools) {
      component_pool.FitIntoPool(id);
    }

    AddComponentsForPrimitive(id, primitive);

    m_world->m_hierarchy.HandleEntityCreation(id);

    for (auto callback : m_entity_created_callbacks) {
      callback(this, id);
    }

    return id;
  }

  //--------------------------------------------------------------
  EntityId EntityManager::CreateMultiMeshEntity(Mesh *mesh) {
    EntityId entity = EntityId::EMPTY;
    if (mesh->GetSubMeshCount() == 1) {
      entity = CreateEntity(EntityPrimitive::Quad);
      GetComponent<Rendering::MeshComponent>(entity)->mesh = mesh;
    } else {
      entity = CreateEntity(EntityPrimitive::Base);
      for (uint32 i = 0; i < mesh->GetSubMeshCount(); i++) {
        EntityId child = CreateEntity(EntityPrimitive::Quad);
        m_world->m_hierarchy.SetParent(child, entity);

        Rendering::MeshComponent *mesh_component = GetComponent<Rendering::MeshComponent>(child);
        mesh_component->mesh = mesh;
        mesh_component->sub_mesh_index = i;
      }
    }

    GetComponent<NameComponent>(entity)->name = "MultiMesh";
    return entity;
  }

  //--------------------------------------------------------------
  EntityId EntityManager::InstantiateEntity(EntityId id) {
    HYP_PROFILE_SCOPE("World.InstantiateEntity");
    
    if (IsAlive(id)) {
      EntityId instantiated = CreateEntity(EntityPrimitive::Empty);

      // First we add all the components our source entity has.
      // Because the adding of components is not pointer-stable we have to delay the actual copying of the data into a second step.
      for (ComponentPool &component_pool : m_storage.component_pools) {
        if (component_pool.HasComponent(id)) {
          const ComponentInfo &component_info = component_pool.GetComponentInfo();
          AddComponent(component_info.id, instantiated);
        }
      }
      
      // Now we are safe to copy over the actual component data.
      for (ComponentPool &component_pool : m_storage.component_pools) {
        void *source_component = component_pool.GetComponent(id);
        if (source_component) {
          const ComponentInfo &component_info = component_pool.GetComponentInfo();
          void *destination_component = component_pool.GetComponent(instantiated);
          component_info.copy_assignment_operator(destination_component, source_component);
        }
      }

      // FIXME: This is currently not correct as it does not copy our potential children. 

      m_world->m_hierarchy.HandleEntityCreation(instantiated);
      
      return instantiated;
    } else {
      HYP_LOG_WARN("Entity", "Trying to instantiate entity from nonexistent entity with id {}.", id);
      return EntityId::EMPTY;
    }
  }

  //--------------------------------------------------------------
  void EntityManager::DestroyEntity(EntityId id, EntityHierarchyDestructionPolicy hierarchy_destruction_policy) {
    HYP_PROFILE_SCOPE("World.DestroyEntity");

    if (IsAlive(id)) {
      for (auto callback : m_entity_destroyed_callbacks) {
        callback(this, id);
      }

      m_world->m_hierarchy.HandleEntityDestruction(id, hierarchy_destruction_policy);

      EntityId new_id = EntityId::Create(m_storage.next, EntityId::GetVersion(id) + 1);
      EntityIndex index = EntityId::GetIndex(id);

      m_storage.next = index;
      m_storage.available++;

      EntityDescription &entity_description = m_storage.entities[index];
      entity_description.id = new_id;

      m_storage.entities_by_guid.Remove(entity_description.guid);

      for (ComponentPool &component_pool : m_storage.component_pools) {
        component_pool.RemoveComponent(id);
      }
    } else {
      HYP_LOG_WARN("Entity", "Trying to destroy nonexistent entity with id {}.", id);
    }
  }

  //--------------------------------------------------------------
  EntityArchetype *EntityManager::CreateArchetype(EntityId id) {
    ArchetypeComponent *archetype_component = GetComponent<ArchetypeComponent>(id);
    if (archetype_component != nullptr) {
      return archetype_component->archetype;
    }

    EntityArchetypeComponentStorage storage;
    for (ComponentPool &component_pool : m_storage.component_pools) {
      const ComponentInfo &component_info = component_pool.GetComponentInfo();
      byte *component = component_pool.GetComponent(id);
      if (component != nullptr) {
        Array<byte> component_data;
        component_data.Resize(component_info.element_size);

        component_info.copy_constructor(component_data.GetData(), component);
        
        storage.Insert(component_info.id, component_data);  
      }
    }

    EntityArchetype *archetype = new EntityArchetype(EntityGuid::Generate(), storage);
    archetype_component = AddComponent<ArchetypeComponent>(id);
    archetype_component->archetype = archetype;

    return archetype;
  }

  //--------------------------------------------------------------
  EntityId EntityManager::InstantiateArchetype(EntityArchetype *archetype) {
    HYP_ASSERT(archetype);

    EntityId copy = CreateEntity(EntityPrimitive::Empty);

    EntityArchetypeComponentStorage storage = archetype->GetStorage();
    for (auto &[component_id, component_data] : storage) {
      const ComponentInfo &component_info = m_storage.component_pools[component_id].GetComponentInfo();
      byte *component = static_cast<byte *>(AddComponent(component_id, copy));
      component_info.copy_constructor(component, component_data.GetData());
    }

    m_world->m_hierarchy.HandleEntityCreation(copy);

    return copy;
  }
  
  //--------------------------------------------------------------
  void EntityManager::SetEnabled(EntityId entity, bool8 enabled) {
    bool8 is_disabled = HasComponent<DisabledComponent>(entity);
    
    if (enabled && is_disabled) {
      RemoveComponent<DisabledComponent>(entity);
    } else if (!is_disabled) {
      AddComponent<DisabledComponent>(entity);
    }
  }

  //--------------------------------------------------------------
  void EntityManager::AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive) {
    if (primitive != EntityPrimitive::Empty) {
      AddComponent<NameComponent>(id)->name = GetNameForPrimitive(primitive);
      AddComponent<LocalTransformComponent>(id);
      AddComponent<DerivedTransformComponent>(id);
      AddComponent<LocalToWorldComponent>(id);
      AddComponent<HierarchyComponent>(id);
    }

    switch (primitive) {
      case EntityPrimitive::Empty: break;
      case EntityPrimitive::Base: break;
      case EntityPrimitive::Camera: AddComponent<Rendering::CameraComponent>(id);
        break;
      case EntityPrimitive::DirectionalLight: AddComponent<Rendering::DirectionalLightComponent>(id);
        break;
      case EntityPrimitive::PointLight: AddComponent<Rendering::PointLightComponent>(id);
        break;
      case EntityPrimitive::SpotLight: AddComponent<Rendering::SpotLightComponent>(id);
        break;
      case EntityPrimitive::Quad:
      case EntityPrimitive::Plane:
      case EntityPrimitive::Cube:
      case EntityPrimitive::Sphere: {
        Rendering::MeshComponent *mesh = AddComponent<Rendering::MeshComponent>(id);
        mesh->material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Default);

        switch (primitive) {
          case EntityPrimitive::Quad: mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad);
            break;
          case EntityPrimitive::Plane: mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Plane);
            break;
          case EntityPrimitive::Cube: {
            mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube);
            AddComponent<Physics::BoxColliderComponent>(id);
            break;
          }
          case EntityPrimitive::Sphere: {
            mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere);
            AddComponent<Physics::SphereColliderComponent>(id);
            break;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            break;
        }

        AddComponent<Rendering::MeshBoundsComponent>(id);

        break;
      }
      case EntityPrimitive::Sprite: AddComponent<Rendering::SpriteComponent>(id);
        break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        break;
    }
  }

  //--------------------------------------------------------------
  String EntityManager::GetNameForPrimitive(EntityPrimitive primitive) {
    switch (primitive) {
      case EntityPrimitive::Empty: return "Empty";
      case EntityPrimitive::Base: return "Entity";
      case EntityPrimitive::Camera: return "Camera";
      case EntityPrimitive::DirectionalLight: return "Directional Light";
      case EntityPrimitive::PointLight: return "Point Light";
      case EntityPrimitive::SpotLight: return "Spot Light";
      case EntityPrimitive::Quad: return "Quad";
      case EntityPrimitive::Plane: return "Plane";
      case EntityPrimitive::Cube: return "Cube";
      case EntityPrimitive::Sphere: return "Sphere";
      case EntityPrimitive::Sprite: return "Sprite";
      default: return "Entity";
    }
  }
  
}
