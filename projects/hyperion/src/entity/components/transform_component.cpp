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
            RecalculateMatricies();

            for (TransformComponent *child : m_children) {
                child->NotifyTransformChange();
            }
        }
    }

    void TransformComponent::NotifyTransformChange() {
        // TODO: We can probably optimize a little if we distinguish between what actually changed
        GetEntity()->OnEvent({ EntityEventType::TransformChanged, nullptr });
    }

    void TransformComponent::RecalculateMatricies() {
        // FIXME: This seems to be a very naive implementation
        std::stack<TransformComponent *> hierarchy;
        TransformComponent *parent = m_parent;
        while (parent != nullptr) {
            hierarchy.push(parent);
            parent = parent->m_parent;
        }

        Mat4 parent_transform = Mat4::Identity();
        while (!hierarchy.empty()) {
            TransformComponent *transform = hierarchy.top();
            hierarchy.pop();
            parent_transform = parent_transform * transform->m_local_to_world_matrix;
        }

        m_local_to_world_matrix = Mat4::TRS(m_position, m_rotation, m_scale) * parent_transform;
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