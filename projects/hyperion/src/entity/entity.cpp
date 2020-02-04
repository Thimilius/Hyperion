#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/mesh_renderer_component.hpp"
#include "hyperion/entity/components/light_component.hpp"
#include "hyperion/assets/asset_library.hpp"

namespace Hyperion {

    void Entity::OnMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::ComponentDestroyed: {
                EntityComponent *component = (EntityComponent *)message.parameter;
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

    Entity *Entity::Create(const String &name, Vec3 position, Quaternion rotation, TransformComponent *parent) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent);
        return entity;
    }

    Entity *Entity::CreatePrimitive(EntityPrimitive primitive) {
        Entity *entity = Create(GetPrimitiveName(primitive));
        
        if (primitive == EntityPrimitive::Sphere || primitive == EntityPrimitive::Cube || primitive == EntityPrimitive::Plane) {
            MeshRendererComponent *renderer = entity->AddComponent<MeshRendererComponent>();
            Ref<Rendering::Mesh> mesh;
            switch (primitive) {
                case EntityPrimitive::Sphere: mesh = AssetLibrary::GetMeshPrimitive(MeshPrimitive::Sphere); break; 
                case EntityPrimitive::Cube: mesh = AssetLibrary::GetMeshPrimitive(MeshPrimitive::Cube); break;
                case EntityPrimitive::Plane: mesh = AssetLibrary::GetMeshPrimitive(MeshPrimitive::Plane); break;
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
            renderer->SetSharedMesh(mesh);
            renderer->SetSharedMaterial(Rendering::Material::Create(AssetLibrary::GetShader("standard_phong")));
        } else if (primitive == EntityPrimitive::Light) {
            entity->AddComponent<LightComponent>();
        }

        return entity;
    }

    void Entity::OnDestroy() {
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            if (it->first != TransformComponent::GetStaticType()) {
                EntityComponent *component = it->second;
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

    void Entity::OnCreate(Vec3 position, Quaternion rotation, TransformComponent *parent) {
        m_world = WorldManager::GetActiveWorld();

        m_components[TransformComponent::GetStaticType()] = &m_transform;
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
            case EntityPrimitive::Sphere: return "Sphere";
            case EntityPrimitive::Cube: return "Cube";
            case EntityPrimitive::Plane: return "Plane";
            case EntityPrimitive::Light: return "Light";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return "Primitive";
        }
    }

}