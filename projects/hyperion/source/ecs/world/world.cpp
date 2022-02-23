//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/components.hpp"
#include "hyperion/physics/physics_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  World::World() {
    m_hierarchy.m_world = this;

    uint64 component_count = ComponentRegistry::GetComponentInfos().GetLength();
    m_storage.component_pools.Reserve(component_count);
    for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
      m_storage.component_pools.Add(ComponentPool(component_info));
    }
    m_storage.component_callbacks.Resize(component_count);

    m_physics_world = Physics::PhysicsEngine::CreateWorld(this);
  }

  //--------------------------------------------------------------
  bool8 World::IsAlive(EntityId id) const {
    EntityIndex index = EntityUtilities::GetIndex(id);
    return index < m_storage.entities.GetLength() && m_storage.entities[index].id == id;
  }

  //--------------------------------------------------------------
  EntityGuid World::GetGuid(EntityId id) const {
    HYP_PROFILE_SCOPE("World.GetGuid");

    if (IsAlive(id)) {
      return m_storage.entities[EntityUtilities::GetIndex(id)].guid;
    } else {
      HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
      return EntityGuid();
    }
  }

  //--------------------------------------------------------------
  EntityId World::GetByGuid(EntityGuid guid) const {
    HYP_PROFILE_SCOPE("World.GetByGuid");

    auto it = m_storage.entities_by_guid.Find(guid);
    if (it == m_storage.entities_by_guid.end()) {
      HYP_LOG_WARN("Entity", "Trying to get id from nonexistent entity with guid {}.", guid.ToString());
      return Entity::EMPTY;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  EntityId World::CreateEntity(EntityPrimitive primitive, EntityGuid guid) {
    HYP_PROFILE_SCOPE("World.CreateEntity");

    EntityId id;
    if (m_storage.available <= 0) {
      m_storage.entities.Add({ EntityUtilities::CreateId(static_cast<EntityIndex>(m_storage.entities.GetLength()), 0), guid });
      id = m_storage.entities.GetLast().id;
    } else {
      EntityIndex new_index = m_storage.next;
      m_storage.next = EntityUtilities::GetIndex(m_storage.entities[new_index].id);
      m_storage.available--;

      EntityId new_id = EntityUtilities::CreateId(new_index, EntityUtilities::GetVersion(m_storage.entities[new_index].id));
      m_storage.entities[new_index].id = new_id;
      m_storage.entities[new_index].guid = guid;

      id = new_id;
    }
    m_storage.entities_by_guid.Insert(guid, id);

    for (ComponentPool &component_pool : m_storage.component_pools) {
      component_pool.FitIntoPool(id);
    }

    AddComponentsForPrimitive(id, primitive);

    m_hierarchy.HandleEntityCreation(id);

    return id;
  }

  //--------------------------------------------------------------
  void World::DestroyEntity(EntityId id, WorldHierarchyDestructionPolicy hierarchy_destruction_policy) {
    HYP_PROFILE_SCOPE("World.DestroyEntity");

    if (IsAlive(id)) {
      m_hierarchy.HandleEntityDestruction(id, hierarchy_destruction_policy);

      EntityId new_id = EntityUtilities::CreateId(m_storage.next, EntityUtilities::GetVersion(id) + 1);
      EntityIndex index = EntityUtilities::GetIndex(id);

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
  EntityArchetype *World::CreateArchetype(EntityId id) {
    ArchetypeComponent *archetype_component = GetComponent<ArchetypeComponent>(id);
    if (archetype_component != nullptr) {
      return archetype_component->archetype;
    }

    EntityArchetypeComponentStorage storage;

    for (const ComponentInfo &component_info : ComponentRegistry::GetComponentInfos()) {
      byte *component = static_cast<byte *>(GetComponent(component_info.id, id));
      if (component != nullptr) {
        Array<byte> component_data(component, component + component_info.element_size);
        storage.Insert(component_info.id, component_data);
      }
    }

    EntityArchetype *archetype = new EntityArchetype(EntityGuid::Generate(), storage);
    archetype_component = AddComponent<ArchetypeComponent>(id);
    archetype_component->archetype = archetype;

    return archetype;
  }

  //--------------------------------------------------------------
  EntityId World::Instantiate(EntityArchetype *archetype) {
    HYP_ASSERT(archetype);

    EntityId copy = CreateEntity(EntityPrimitive::Empty);

    EntityArchetypeComponentStorage storage = archetype->GetStorage();
    for (auto &[component_id, component_data] : storage) {
      uint64 element_size = m_storage.component_pools[component_id].GetComponentInfo().element_size;
      byte *component = static_cast<byte *>(AddComponent(component_id, copy));
      std::memcpy(component, component_data.GetData(), element_size);
    }

    m_hierarchy.HandleEntityCreation(copy);

    return copy;
  }

  //--------------------------------------------------------------
  void World::AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive) {
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
      case EntityPrimitive::Camera: AddComponent<Rendering::CameraComponent>(id); break;
      case EntityPrimitive::DirectionalLight: AddComponent<Rendering::DirectionalLightComponent>(id); break;
      case EntityPrimitive::PointLight: AddComponent<Rendering::PointLightComponent>(id); break;
      case EntityPrimitive::SpotLight: AddComponent<Rendering::SpotLightComponent>(id); break;
      case EntityPrimitive::Quad:
      case EntityPrimitive::Plane:
      case EntityPrimitive::Cube:
      case EntityPrimitive::Sphere:
      {
        Rendering::MeshComponent *mesh = AddComponent<Rendering::MeshComponent>(id);
        mesh->material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Default);

        switch (primitive) {
          case EntityPrimitive::Quad: mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad); break;
          case EntityPrimitive::Plane: mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Plane); break;
          case EntityPrimitive::Cube:
          {
            mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube);
            AddComponent<Physics::BoxColliderComponent>(id);
            break;
          }
          case EntityPrimitive::Sphere:
          {
            mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere);
            AddComponent<Physics::SphereColliderComponent>(id);
            break;
          }
          default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }

        BoundingBox bounds = mesh->mesh->GetBounds();
        AddComponent<Rendering::LocalMeshBoundsComponent>(id)->bounds = bounds;
        AddComponent<Rendering::WorldMeshBoundsComponent>(id)->bounds = bounds;

        break;
      }
      case EntityPrimitive::Sprite: AddComponent<Rendering::SpriteComponent>(id); break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
  }

  //--------------------------------------------------------------
  String World::GetNameForPrimitive(EntityPrimitive primitive) {
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