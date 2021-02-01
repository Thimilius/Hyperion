#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"

namespace Hyperion {

    bool Entity::IsActiveInHierarchy() const {
        // TODO: Maybe this is something we should cache instead of recomputing it every time.
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

    Component *Entity::AddComponent(Type type) {
        HYP_ASSERT(type.is_valid());
        HYP_ASSERT_MESSAGE(m_components.find(type) == m_components.end(), "Failed to add component because a component with the same type already exists!");

        auto constructor = type.get_constructor();
        HYP_ASSERT_MESSAGE(constructor.is_valid(), "Failed to add component because the component does not have a valid default constructor!");
        Variant variant = constructor.invoke();
        HYP_ASSERT(variant.is_valid() && variant.get_type().is_derived_from<Component>());

        Component *component = variant.get_value<Component *>();
        component->m_entity = this;
        m_components[type] = component;

        component->OnCreate();

        return component;
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

    Entity *Entity::Create() {
        return new Entity();
    }

    Entity *Entity::Create(const String &name, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent, world);
        return entity;
    }

    Entity *Entity::CreatePrimitive(EntityPrimitive primitive, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = Create(GetPrimitiveName(primitive), position, rotation, parent, world);
        
        return entity;
    }

    void Entity::OnDestroy() {
        // First destroy every child.
        if (!m_transform->m_children.empty()) {
            for (int32 i = (static_cast<int32>(m_transform->m_children.size())) - 1; i >= 0; i--) {
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

        // Now destroy all our components except transform.
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            auto [component_type, component] = *it;
            if (component_type.is_derived_from<Transform>()) {
                ++it;
            } else {
                it = m_components.erase(it);
                DestroyImmediate(component);
            }
        }

        // At the very end we can destroy the transform.
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
