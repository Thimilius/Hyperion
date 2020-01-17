#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/scene.hpp"

namespace Hyperion {

    void Entity::OnEvent(EntityEvent event) {
        switch (event.type) {
            case EntityEventType::ComponentDestroyed: {
                EntityComponent *component = (EntityComponent *)event.parameter;
                component->OnDestroy();
                m_components.erase(component->GetType());
                break;
            }
        }

        for (EntityEventListener *listener : m_event_listeners) {
            listener->OnEvent(event);
        }
    }

    Entity *Entity::Create(const String &name) {
        return new Entity(name);
    }

    Entity::Entity(const String &name) : Object(name) { 
        m_components[TransformComponent::GetStaticType()] = &m_transform;
        m_transform.m_entity = this;

        m_scene = SceneManager::GetActiveScene();
        m_scene->AddRootEntity(this);
    }

    Entity::~Entity() {
        for (auto pair : m_components) {
            if (pair.first != TransformComponent::GetStaticType()) {
                Object::Destroy(pair.second);
            }
        }

        m_scene->OnEntityDestroyed(this);
    }

}