#include "hyppch.hpp"

#include "hyperion/entity/components/transform_component.hpp"

#include <stack>

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/scene.hpp"

namespace Hyperion {

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

            entity->GetScene()->AddRootEntity(entity);
        } else {
            parent->m_children.push_back(this);

            entity->GetScene()->RemoveRootEntity(entity);
        }

        m_parent = parent;

        NotifyTransformChange();
    }

    void TransformComponent::OnEvent(EntityEvent event) {
        if (event.type == EntityEventType::TransformChanged) {
            RecalculateTransform();
            RecalculateMatricies();

            for (TransformComponent *child : m_children) {
                child->NotifyTransformChange();
            }
        }
    }

    void TransformComponent::NotifyTransformChange() {
        GetEntity()->OnEvent({ EntityEventType::TransformChanged, nullptr });
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
    }

    void TransformComponent::RecalculateMatricies() {
        m_local_to_world_matrix = Mat4::TRS(m_derived_position, m_derived_rotation, m_derived_scale);
        m_world_to_local_matrix = m_local_to_world_matrix.Inverted();
    }

    void TransformComponent::OnCreate() {
        GetEntity()->RegisterEventListener(this);
    }

    void TransformComponent::OnDestroy() {
        Entity *entity = GetEntity();
        entity->UnregisterEventListener(this);

        if (m_parent == nullptr) {
            entity->GetScene()->RemoveRootEntity(entity);
        } else {
            auto begin = m_parent->m_children.begin();
            auto end = m_parent->m_children.end();
            if (std::find(begin, end, this) != end) {
                m_parent->m_children.erase(std::remove(begin, end, this));
            }
        }
    }

}