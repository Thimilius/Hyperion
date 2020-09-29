#include "hyppch.hpp"

#include "hyperion/entity/components/ui/ui_transform.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    void UITransform::GetWorldCorners(Vec3 corners[4]) {
        f32 w = m_size.x;
        f32 h = m_size.y;
        f32 p_x = m_pivot.x;
        f32 p_y = m_pivot.y;

        corners[0] = m_local_to_world_matrix * Vec4((1.0f - p_x) * w, (1.0f - p_y) * h, 0.0f, 1.0f);
        corners[1] = m_local_to_world_matrix * Vec4((1.0f - p_x) * w, -p_y * h        , 0.0f, 1.0f);
        corners[2] = m_local_to_world_matrix * Vec4(-p_x * w        , -p_y * h        , 0.0f, 1.0f);
        corners[3] = m_local_to_world_matrix * Vec4(-p_x * w        , (1.0f - p_y) * h, 0.0f, 1.0f);
    }

    bool UITransform::IsPointInRect(Vec2 point) {
        Vec3 world_corners[4];
        GetWorldCorners(world_corners);

        Vec2 p1 = world_corners[0];
        Vec2 p2 = world_corners[1];
        Vec2 p3 = world_corners[2];
        Vec2 p4 = world_corners[3];

        // NOTE: Counter clockwise order of points is important
        return (IsLeft(p1, p4, point) > 0 && IsLeft(p4, p3, point) > 0 && IsLeft(p3, p2, point) > 0 && IsLeft(p2, p1, point) > 0);
    }

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

        // Update the parent
        Transform *parent = entity->m_transform->m_parent;
        if (parent) {
            auto begin = parent->m_children.begin();
            auto end = parent->m_children.end();
            auto pos = std::find(begin, end, obsolete);
            if (pos != end) {
                u64 index = std::distance(begin, pos);
                parent->m_children[index] = this;
            }
        }

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

    f32 UITransform::IsLeft(Vec2 p0, Vec2 p1, Vec2 p2) {
        return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    }

}

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;

    registration::class_<UITransform>(HYP_NAME_OF_TYPE(UITransform));
}
