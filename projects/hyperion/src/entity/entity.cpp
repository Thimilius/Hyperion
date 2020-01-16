#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/scene.hpp"

namespace Hyperion {

    Entity *Entity::Create(const String &name) {
        return new Entity(name);
    }

    Entity::Entity(const String &name) : Object(name) { 
        m_scene = SceneManager::GetActiveScene();
        m_scene->AddRootEntity(this);
    }

    Entity::~Entity() {
        if (!m_destroyed) {
            for (auto pair : m_components) {
                if (pair.first != TransformComponent::GetStaticType()) {
                    Object::Destroy(pair.second);
                }
            }

            m_scene->OnEntityDestroyed(this);
        }
    }

    void Entity::OnComponentDestroyed(EntityComponent *component) {
        m_components.erase(component->GetType());
    }

}