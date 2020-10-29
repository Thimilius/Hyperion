#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/light.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/assets/asset_manager.hpp"

namespace Hyperion {

    bool Entity::IsActiveInHierarchy() const {
        // TODO: Maybe this is something we should cache instead of recomputing it every time
        bool active_self = m_active;
        if (!active_self) {
            return false;
        }

        Transform *parent = m_transform->m_parent;
        while (parent != nullptr) {
            if (!parent->GetEntity()->m_active) {
                return false;
            }
            parent = parent->m_parent;
        }

        return true;
    }

    void Entity::DispatchMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::ComponentDestroyed: {
                Component *component = static_cast<Component *>(message.parameter);
                m_components.erase(component->GetType());
                break;
            }
        }

        for (IEntityMessageListener *listener : m_message_listeners) {
            listener->OnMessage(message);
        }
    }

    void Entity::RegisterMessageListener(IEntityMessageListener *listener) {
        m_message_listeners.push_back(listener);
    }

    void Entity::UnregisterMessageListener(IEntityMessageListener *listener) {
        auto begin = m_message_listeners.begin();
        auto end = m_message_listeners.end();
        if (std::find(begin, end, listener) != end) {
            m_message_listeners.erase(std::remove(begin, end, listener));
        }
    }

    Entity *Entity::Create(const String &name, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent, world);
        return entity;
    }

    Entity *Entity::CreatePrimitive(EntityPrimitive primitive, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = Create(GetPrimitiveName(primitive), position, rotation, parent, world);
        
        if (primitive == EntityPrimitive::Quad || primitive == EntityPrimitive::Plane || primitive == EntityPrimitive::Cube || primitive == EntityPrimitive::Sphere) {
            MeshRenderer *renderer = entity->AddComponent<MeshRenderer>();
            Rendering::Mesh *mesh;
            switch (primitive) {
                case EntityPrimitive::Quad: {
                    mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad);
                    break;
                }
                case EntityPrimitive::Plane: {
                    mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Plane);
                    break;
                }
                case EntityPrimitive::Cube: {
                    entity->AddComponent<BoxCollider>();
                    mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Cube);
                    break;
                }
                case EntityPrimitive::Sphere: {
                    entity->AddComponent<SphereCollider>();
                    mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere);
                    break;
                }
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
        // First destroy every child
        if (!m_transform->m_children.empty()) {
            for (s32 i = ((s32)m_transform->m_children.size()) - 1; i >= 0; i--) {
                Entity *child = m_transform->m_children[i]->m_entity;
                // Children already scheduled for destruction get seperated from us by removing the parent reference.
                // This way the order of destruction is not important.
                if (child->m_destroyed) {
                    child->m_transform->m_parent = nullptr;
                } else {
                    DestroyImmediate(child);
                }
            }
        }

        // Now destroy all our components except transform
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            auto [component_type, component] = *it;
            if (component_type.is_derived_from<Transform>()) {
                ++it;
            } else {
                it = m_components.erase(it);
                DestroyImmediate(component);
            }
        }

        // At the very end we can destroy the transform
        if (m_transform->GetType() == rttr::type::get<RectTransform>()) {
            static_cast<RectTransform *>(m_transform)->m_replace_on_destroy = false;
        }
        DestroyImmediate(m_transform);
    }

    void Entity::NotifyActivationChanged() {
        DispatchMessage({ EntityMessageType::ActivationChanged, nullptr });

        for (Transform *child : m_transform->m_children) {
            child->GetEntity()->NotifyActivationChanged();
        }
    }

    void Entity::OnCreate(const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        if (parent) {
            world = parent->GetWorld();
        } else if (!world) {
            world = WorldManager::GetActiveWorld();
        }
        m_world = world;

        m_transform = new Transform();

        m_transform->m_entity = this;
        m_transform->m_local_position = position;
        m_transform->m_derived_position = position;
        m_transform->m_local_rotation = rotation;
        m_transform->m_derived_rotation = rotation;
        m_transform->OnCreate();

        m_components[rttr::type::get<Transform>()] = m_transform;

        if (parent) {
            m_transform->SetParent(parent);
        } else {
            m_world->AddRootEntity(this);
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

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<Entity>(HYP_NAME_OF_TYPE(Entity));
}
