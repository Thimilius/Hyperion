#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_transform.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    void UITransform::OnCreate() {
        // Creating a ui transform means replacing the current one
        Transform *obsolete = GetEntity()->m_transform;

        // We need to copy everything over from the now obsolete transform we are about to destroy
        m_local_position = obsolete->m_local_position;
        m_local_rotation = obsolete->m_local_rotation;
        m_local_scale = obsolete->m_local_scale;
        m_derived_position = obsolete->m_derived_position;
        m_derived_rotation = obsolete->m_derived_rotation;
        m_derived_scale = obsolete->m_derived_scale;
        m_local_to_world_matrix = obsolete->m_local_to_world_matrix;
        m_world_to_local_matrix = obsolete->m_world_to_local_matrix;
        m_parent = obsolete->m_parent;
        m_children = std::move(obsolete->m_children);

        // Update the entity
        Entity *entity = GetEntity();
        entity->m_transform = this;
        entity->m_components.erase(rttr::type::get<Transform>());
        entity->m_components[rttr::type::get<UITransform>()] = this;

        // Update the children
        for (Transform *child : m_children) {
            child->m_parent = this;
        }

        // We can now delete the obsolete transform component that is no longer being used
        // NOTE: This is propbably a little unsafe just deleting the component like that but works for now
        delete obsolete;
    }

    void UITransform::OnDestroy() {
        if (m_replace_on_destroy) {
            Transform *transform = new Transform();

            // We need to copy everything over from the now obsolete transform we are about to destroy
            transform->m_local_position = m_local_position;
            transform->m_local_rotation = m_local_rotation;
            transform->m_local_scale = m_local_scale;
            transform->m_derived_position = m_derived_position;
            transform->m_derived_rotation = m_derived_rotation;
            transform->m_derived_scale = m_derived_scale;
            transform->m_local_to_world_matrix = m_local_to_world_matrix;
            transform->m_world_to_local_matrix = m_world_to_local_matrix;
            transform->m_parent = m_parent;
            transform->m_children = std::move(m_children);

            // Update the entity
            Entity *entity = GetEntity();
            transform->m_entity = entity;
            entity->m_transform = transform;
            entity->m_components.erase(rttr::type::get<UITransform>());
            entity->m_components[rttr::type::get<Transform>()] = transform;

            // Update the children
            for (Transform *child : m_children) {
                child->m_parent = transform;
            }
        } else {
            Transform::OnDestroy();
        }
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UITransform>(String(NAMEOF_SHORT_TYPE(UITransform)));
}
