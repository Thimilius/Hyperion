//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/world/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    World::World() {
        m_hierarchy.m_world = this;

        for (const ComponentInfo &component_info : ComponentRegistry::GetComponents()) {
            m_storage.component_pools.Add(ComponentPool(component_info));
        }
    }

    //--------------------------------------------------------------
    bool8 World::IsAlive(EntityId id) const {
        EntityIndex index = EntityUtilities::GetIndex(id);
        return index < m_storage.entities.GetLength() && m_storage.entities[index].id == id;
    }

    //--------------------------------------------------------------
    EntityGuid World::GetGuid(EntityId id) const {
        if (IsAlive(id)) {
            return m_storage.entities[EntityUtilities::GetIndex(id)].guid;
        } else {
            HYP_LOG_WARN("Entity", "Trying to get GUID from nonexistent entity with id {}.", id);
            return EntityGuid();
        }
    }

    //--------------------------------------------------------------
    EntityId World::GetByGuid(EntityGuid guid) const {
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
    void World::AddComponentsForPrimitive(EntityId id, EntityPrimitive primitive) {
        if (primitive != EntityPrimitive::Empty) {
            AddComponent<TagComponent>(id);
            AddComponent<LocalTransformComponent>(id);
            AddComponent<DerivedTransformComponent>(id);
            AddComponent<LocalToWorldComponent>(id);
            AddComponent<HierarchyComponent>(id);
        }

        switch (primitive) {
            case EntityPrimitive::Empty: break;
            case EntityPrimitive::Base: break;
            case EntityPrimitive::Quad:
            case EntityPrimitive::Plane:
            case EntityPrimitive::Cube:
            case EntityPrimitive::Sphere: {
                Rendering::RenderMeshComponent *render_mesh = AddComponent<Rendering::RenderMeshComponent>(id);
                render_mesh->material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Default);

                switch (primitive) {
                    case EntityPrimitive::Quad: render_mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad); break;
                    case EntityPrimitive::Plane: render_mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Plane); break;
                    case EntityPrimitive::Cube: render_mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube); break;
                    case EntityPrimitive::Sphere: render_mesh->mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere); break;
                    default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
                }

                break;
            }
            case EntityPrimitive::Camera: AddComponent<Rendering::CameraComponent>(id); break;
            case EntityPrimitive::Sprite: AddComponent<Rendering::SpriteComponent>(id); break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

}