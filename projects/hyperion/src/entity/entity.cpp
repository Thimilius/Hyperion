#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/light.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/assets/asset_manager.hpp"

namespace Hyperion {

    void Entity::OnMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::ComponentDestroyed: {
                Component *component = (Component *)message.parameter;
                m_components.erase(component->GetType());
                break;
            }
        }

        for (EntityMessageListener *listener : m_message_listeners) {
            listener->OnMessage(message);
        }
    }

    void Entity::RegisterMessageListener(EntityMessageListener *listener) {
        m_message_listeners.push_back(listener);
    }

    void Entity::UnregisterMessageListener(EntityMessageListener *listener) {
        auto begin = m_message_listeners.begin();
        auto end = m_message_listeners.end();
        if (std::find(begin, end, listener) != end) {
            m_message_listeners.erase(std::remove(begin, end, listener));
        }
    }

    Entity *Entity::Create(const String &name, Vec3 position, Quaternion rotation, Transform *parent, World *world) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent, world);
        return entity;
    }

    Entity *Entity::CreatePrimitive(EntityPrimitive primitive) {
        Entity *entity = Create(GetPrimitiveName(primitive));
        
        if (primitive == EntityPrimitive::Quad || primitive == EntityPrimitive::Plane || primitive == EntityPrimitive::Cube || primitive == EntityPrimitive::Sphere) {
            MeshRenderer *renderer = entity->AddComponent<MeshRenderer>();
            Ref<Rendering::Mesh> mesh;
            switch (primitive) {
                case EntityPrimitive::Quad: mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad); break;
                case EntityPrimitive::Plane: mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Plane); break;
                case EntityPrimitive::Cube: mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube); break;
                case EntityPrimitive::Sphere: mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere); break; 
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
            renderer->SetSharedMesh(mesh);
            renderer->SetSharedMaterial(AssetManager::GetDefaultMaterial());
        } else if (primitive == EntityPrimitive::PointLight || primitive == EntityPrimitive::DirectionalLight || primitive == EntityPrimitive::SpotLight) {
            Light *light = entity->AddComponent<Light>();
            LightType light_type;
            switch (primitive) {
                case Hyperion::EntityPrimitive::DirectionalLight: light_type = LightType::Directional; break;
                case Hyperion::EntityPrimitive::PointLight: light_type = LightType::Point; break;
                case Hyperion::EntityPrimitive::SpotLight: light_type = LightType::Spot; break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
            light->SetLightType(light_type);
        } else if (primitive == EntityPrimitive::Camera) {
            entity->AddComponent<Camera>();
        }

        return entity;
    }

    void Entity::OnDestroy() {
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            if (it->first != Transform::GetTypeStatic()) {
                Component *component = it->second;
                it = m_components.erase(it);
                DestroyImmediate(component);
            } else {
                ++it;
            }
        }
        m_transform.OnDestroy();

        if (!m_transform.m_children.empty()) {
            for (s32 i = ((s32)m_transform.m_children.size()) - 1; i >= 0; i--) {
                Entity *child = m_transform.m_children[i]->m_entity;
                // Children already scheduled for destruction get seperated from us by removing the parent reference.
                // This way the order of destruction is not important.
                if (child->m_destroyed) {
                    child->m_transform.m_parent = nullptr;
                } else {
                    DestroyImmediate(child);
                }
            }
        }
    }

    void Entity::OnCreate(Vec3 position, Quaternion rotation, Transform *parent, World *world) {
        if (!world) {
            world = WorldManager::GetActiveWorld();
        }
        m_world = world;

        m_components[Transform::GetTypeStatic()] = &m_transform;
        m_transform.m_entity = this;
        m_transform.OnCreate();
        m_transform.SetLocalPosition(position);
        m_transform.SetLocalRotation(rotation);

        if (parent == nullptr) {
            m_world->AddRootEntity(this);
        } else {
            m_transform.SetParent(parent);
        }
    }

    String Entity::GetPrimitiveName(EntityPrimitive primitive) {
        switch (primitive) {
            case EntityPrimitive::Quad: return "Quad";
            case EntityPrimitive::Plane: return "Plane";
            case EntityPrimitive::Cube: return "Cube";
            case EntityPrimitive::Sphere: return "Sphere";
            case EntityPrimitive::DirectionalLight: return "Directional Light";
            case EntityPrimitive::PointLight: return "Point Light";
            case EntityPrimitive::SpotLight: return "Spot Light";
            case EntityPrimitive::Camera: return "Camera";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return "Primitive";
        }
    }

}