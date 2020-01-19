#include "hyppch.hpp"

#include "hyperion/entity/entity.hpp"

#include "hyperion/entity/world.hpp"

namespace Hyperion {

    void Entity::OnEvent(EntityEvent event) {
        switch (event.type) {
            case EntityEventType::ComponentDestroyed: {
                EntityComponent *component = (EntityComponent *)event.parameter;
                m_components.erase(component->GetType());
                break;
            }
        }

        for (EntityEventListener *listener : m_event_listeners) {
            listener->OnEvent(event);
        }
    }

    void Entity::OnCreate() {
        m_components[TransformComponent::GetStaticType()] = &m_transform;
        m_transform.m_entity = this;
        m_transform.OnCreate();

        m_world = WorldManager::GetActiveWorld();
        m_world->AddRootEntity(this);
    }

    void Entity::OnDestroy() {
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            if (it->first != TransformComponent::GetStaticType()) {
                DestroyImmediate(it->second);
                it++;
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

    Entity *Entity::Create(const String &name) {
        Entity *entity = new Entity(name);
        entity->OnCreate();
        return entity;
    }

}