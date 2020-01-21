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

    void Entity::RegisterEventListener(EntityEventListener *listener) {
        m_event_listeners.push_back(listener);
    }

    void Entity::UnregisterEventListener(EntityEventListener *listener) {
        auto begin = m_event_listeners.begin();
        auto end = m_event_listeners.end();
        if (std::find(begin, end, listener) != end) {
            m_event_listeners.erase(std::remove(begin, end, listener));
        }
    }

    Entity *Entity::Create(const String &name, Vec3 position, Quaternion rotation, TransformComponent *parent) {
        Entity *entity = new Entity(name);
        entity->OnCreate(position, rotation, parent);
        return entity;
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

}