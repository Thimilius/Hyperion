//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/entity.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool Entity::IsActiveInHierarchy() const {
        if (!m_active) {
            return false;
        } else {
            Transform *parent = m_transform->m_parent;
            if (parent != nullptr) {
                return parent->GetEntity()->IsActiveInHierarchy();
            } else {
                return true;
            }
        }
    }

    //--------------------------------------------------------------
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

    //--------------------------------------------------------------
    Script *Entity::AddScript(Scripting::ScriptingType *scripting_type) {
        Script *script = Script::Create();
        script->m_entity = this;
        script->m_scripting_type = scripting_type;
        m_components[scripting_type] = script;

        script->OnCreate();

        return script;
    }

    //--------------------------------------------------------------
    Component *Entity::GetComponent(ComponentType type) const {
        for (auto [component_type, component] : m_components) {
            if (component_type.IsDerivedFrom(type)) {
                return component;
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------
    void Entity::DispatchMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::ComponentDestroyed: {
                m_components.erase(message.data.component_destroyed.component->GetType());
                break;
            }
            case EntityMessageType::ScriptDestroyed: {
                m_components.erase(message.data.script_destroyed.script->GetScriptingType());
                break;
            }
        }

        for (IEntityMessageListener *listener : m_message_listeners) {
            listener->OnMessage(message);
        }
    }

    //--------------------------------------------------------------
    void Entity::RegisterMessageListener(IEntityMessageListener *listener) {
        m_message_listeners.push_back(listener);
    }

    //--------------------------------------------------------------
    void Entity::UnregisterMessageListener(IEntityMessageListener *listener) {
        auto begin = m_message_listeners.begin();
        auto end = m_message_listeners.end();
        if (std::find(begin, end, listener) != end) {
            m_message_listeners.erase(std::remove(begin, end, listener));
        }
    }

    //--------------------------------------------------------------
    Entity *Entity::Create() {
        return new Entity();
    }

    //--------------------------------------------------------------
    Entity *Entity::Create(const String &name, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent, world);
        return entity;
    }

    //--------------------------------------------------------------
    Entity *Entity::CreatePrimitive(EntityPrimitive primitive, const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        Entity *entity = Create(GetPrimitiveName(primitive), position, rotation, parent, world);
        
        MeshRenderer *mesh_renderer = nullptr;
        switch (primitive) {
            case EntityPrimitive::Quad: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Quad));
                break;
            }
            case EntityPrimitive::Plane: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Plane));
                break;
            }
            case EntityPrimitive::Cube: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Cube));
                entity->AddComponent<BoxCollider>();
                break;
            }
            case EntityPrimitive::Sphere: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere));
                entity->AddComponent<SphereCollider>();
                break;
            }
            case EntityPrimitive::DirectionalLight: break;
            case EntityPrimitive::PointLight: break;
            case EntityPrimitive::SpotLight: break;
            case EntityPrimitive::Camera: {
                entity->AddComponent<Camera>();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        if (mesh_renderer != nullptr) {
            mesh_renderer->SetMaterial(AssetManager::GetDefaultMaterial());
        }

        return entity;
    }

    //--------------------------------------------------------------
    void Entity::OnDestroy() {
        // First destroy every child.
        if (!m_transform->m_children.empty()) {
            for (int32 i = (static_cast<int32>(m_transform->m_children.size())) - 1; i >= 0; i--) {
                Entity *child = m_transform->m_children[i]->m_entity;
                // Children already scheduled for destruction get seperated from us by removing the parent reference.
                // This way the order of destruction is not important.
                if (child->IsDestroyed()) {
                    child->m_transform->m_parent = nullptr;
                } else {
                    DestroyImmediate(child);
                }
            }
        }

        // Now destroy all our components except transform.
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            auto [component_type, component] = *it;
            if (component_type.IsDerivedFrom(Type::get<Transform>())) {
                ++it;
            } else {
                it = m_components.erase(it);
                DestroyImmediate(component);
            }
        }

        if (m_transform->GetType() == Type::get<RectTransform>()) {
            static_cast<RectTransform *>(m_transform)->m_replace_on_destroy = false;
        }
        // At the very end we can destroy the transform.
        DestroyImmediate(m_transform);
    }

    //--------------------------------------------------------------
    void Entity::NotifyActivationChanged() {
        DispatchMessage({ EntityMessageType::ActivationChanged, nullptr });

        for (Transform *child : m_transform->m_children) {
            child->GetEntity()->NotifyActivationChanged();
        }
    }

    //--------------------------------------------------------------
    void Entity::OnCreate(const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world) {
        if (parent) {
            world = parent->GetWorld();
        } else if (!world) {
            world = WorldManager::GetActiveWorld();
        }
        HYP_ASSERT(world);
        m_world = world;

        m_transform = new Transform();

        m_transform->m_entity = this;
        m_transform->m_local_position = position;
        m_transform->m_derived_position = position;
        m_transform->m_local_rotation = rotation;
        m_transform->m_derived_rotation = rotation;
        m_transform->OnCreate();

        m_components[Type::get<Transform>()] = m_transform;

        if (parent) {
            m_transform->SetParent(parent);
        } else {
            m_world->AddRootEntity(this);
        }
    }

    //--------------------------------------------------------------
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
