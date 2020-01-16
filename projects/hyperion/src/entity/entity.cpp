#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/scene.hpp"

namespace Hyperion {

    // TODO: Rework memory management of entities
    void Entity::Destroy() {
        if (!m_destroyed) {
            for (auto pair : m_components) {
                delete pair.second;
            }
            m_components.clear();

            m_scene->OnEntityDestroyed(this);

            m_destroyed = true;
            delete this;
        }
    }

    void Entity::Destroy(Entity *entity) {
        entity->Destroy();
    }

    Entity *Entity::Create(const String &name) {
        return new Entity(name);
    }

    Entity::Entity(const String &name) : Asset(name) { 
        m_scene = SceneManager::GetActiveScene();
        m_scene->AddRootEntity(this);
    }

}