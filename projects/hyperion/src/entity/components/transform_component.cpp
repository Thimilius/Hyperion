#include "hyppch.hpp"

#include "hyperion/entity/components/transform_component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    TransformComponent *TransformComponent::GetRoot() const {
        TransformComponent *parent = m_parent ? m_parent : (TransformComponent *)this;
        while (parent->m_parent != nullptr) {
            parent = parent->m_parent;
        }
        return parent;
    }

    void TransformComponent::SetParent(TransformComponent *parent) {
        if (m_parent == parent) {
            return;
        }
        
        Entity *entity = GetEntity();
        if (parent == nullptr) {
            auto begin = m_parent->m_children.begin();
            auto end = m_parent->m_children.end();
            if (std::find(begin, end, this) != end) {
                m_parent->m_children.erase(std::remove(begin, end, this));
            }

            entity->GetWorld()->AddRootEntity(entity);
        } else {
            parent->m_children.push_back(this);

            entity->GetWorld()->RemoveRootEntity(entity);
        }

        m_parent = parent;

        NotifyTransformChange();
    }

    void TransformComponent::OnMessage(EntityMessage message) {
        if (message.type == EntityMessageType::TransformChanged) {
            RecalculateTransform();

            for (TransformComponent *child : m_children) {
                child->NotifyTransformChange();
            }
        }
    }

    void TransformComponent::NotifyTransformChange() {
        GetEntity()->OnMessage({ EntityMessageType::TransformChanged, nullptr });
    }

    void TransformComponent::RecalculateTransform() {
        if (m_parent) {
            m_derived_rotation = m_parent->m_derived_rotation * m_local_rotation;
            m_derived_scale = m_parent->m_derived_scale * m_local_scale;

            m_derived_position = m_parent->m_derived_rotation * (m_parent->m_derived_scale * m_local_position);
            m_derived_position += m_parent->m_derived_position;
        } else {
            m_derived_position = m_local_position;
            m_derived_rotation = m_local_rotation;
            m_derived_scale = m_local_scale;
        }

        m_local_to_world_matrix = Mat4::TRS(m_derived_position, m_derived_rotation, m_derived_scale);
        m_world_to_local_matrix = m_local_to_world_matrix.Inverted();
    }

    void TransformComponent::OnCreate() {
        GetEntity()->RegisterMessageListener(this);
    }

    void TransformComponent::OnDestroy() {
        Entity *entity = GetEntity();
        entity->UnregisterMessageListener(this);

        if (m_parent == nullptr) {
            entity->GetWorld()->RemoveRootEntity(entity);
        } else {
            auto begin = m_parent->m_children.begin();
            auto end = m_parent->m_children.end();
            if (std::find(begin, end, this) != end) {
                m_parent->m_children.erase(std::remove(begin, end, this));
            }
        }
    }

}