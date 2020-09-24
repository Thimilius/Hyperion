#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_transform.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    void UITransform::OnCreate() {
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

        GetEntity()->m_transform = this;
    }

    void UITransform::OnDestroy() {

    }

}