//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rect_transform.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void RectTransform::SetAnchoringPreset(AnchoringPreset anchoring_preset) {
        switch (anchoring_preset) {
            case AnchoringPreset::Center: {
                m_anchor_min = Vec2(0.5f, 0.5f);
                m_anchor_max = Vec2(0.5f, 0.5f);
                m_pivot = Vec2(0.5f, 0.5f);
                break;
            }
            case AnchoringPreset::RightCenter: {
                m_anchor_min = Vec2(1.0f, 0.5f);
                m_anchor_max = Vec2(1.0f, 0.5f);
                m_pivot = Vec2(1.0f, 0.5f);
                break;
            }
            case AnchoringPreset::LeftCenter: {
                m_anchor_min = Vec2(0.0f, 0.5f);
                m_anchor_max = Vec2(0.0f, 0.5f);
                m_pivot = Vec2(0.0f, 0.5f);
                break;
            }
            case AnchoringPreset::TopCenter: {
                m_anchor_min = Vec2(0.5f, 1.0f);
                m_anchor_max = Vec2(0.5f, 1.0f);
                m_pivot = Vec2(0.5f, 1.0f);
                break;
            }
            case AnchoringPreset::BottomCenter: {
                m_anchor_min = Vec2(0.5f, 0.0f);
                m_anchor_max = Vec2(0.5f, 0.0f);
                m_pivot = Vec2(0.5f, 0.0f);
                break;
            }
            case AnchoringPreset::TopRightCorner: {
                m_anchor_min = Vec2(1.0f, 1.0f);
                m_anchor_max = Vec2(1.0f, 1.0f);
                m_pivot = Vec2(1.0f, 1.0f);
                break;
            }
            case AnchoringPreset::TopLeftCorner: {
                m_anchor_min = Vec2(0.0f, 1.0f);
                m_anchor_max = Vec2(0.0f, 1.0f);
                m_pivot = Vec2(0.0f, 1.0f);
                break;
            }
            case AnchoringPreset::BottomRightCorner: {
                m_anchor_min = Vec2(1.0f, 0.0f);
                m_anchor_max = Vec2(1.0f, 0.0f);
                m_pivot = Vec2(1.0f, 0.0f);
                break;
            }
            case AnchoringPreset::BottomLeftCorner: {
                m_anchor_min = Vec2(0.0f, 0.0f);
                m_anchor_max = Vec2(0.0f, 0.0f);
                m_pivot = Vec2(0.0f, 0.0f);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

    //--------------------------------------------------------------
    void RectTransform::GetWorldCorners(Vec3 corners[4]) {
        float32 w = m_size.x;
        float32 h = m_size.y;
        float32 p_x = m_pivot.x;
        float32 p_y = m_pivot.y;

        Vec2 parent_size;
        if (m_parent && m_parent->GetType() == Type::get<RectTransform>()) {
            RectTransform *parent_transform = static_cast<RectTransform *>(m_parent);
            parent_size = parent_transform->m_size;
        } else {
            parent_size = Vec2(static_cast<float32>(Display::GetWidth()), static_cast<float32>(Display::GetHeight()));
        }
        Vec2 half_parent_size = parent_size / 2.0f;

        // This comparison is most likely very unreliable
        bool stretching = m_anchor_min != m_anchor_max;
        if (!stretching) {
            float32 x = m_anchor_min.x * parent_size.x - half_parent_size.x;
            float32 y = m_anchor_min.y * parent_size.y - half_parent_size.y;

            float32 offset_x = m_derived_scale.x * m_anchored_position.x;
            float32 offset_y = m_derived_scale.y * m_anchored_position.y;

            SetPosition(Vec3(x + offset_x, y + offset_y, 0.0f));
        }

        corners[0] = m_local_to_world_matrix * Vec4((1.0f - p_x) * w, (1.0f - p_y) * h, 0.0f, 1.0f);
        corners[1] = m_local_to_world_matrix * Vec4((1.0f - p_x) * w, -p_y * h, 0.0f, 1.0f);
        corners[2] = m_local_to_world_matrix * Vec4(-p_x * w, -p_y * h, 0.0f, 1.0f);
        corners[3] = m_local_to_world_matrix * Vec4(-p_x * w, (1.0f - p_y) * h, 0.0f, 1.0f);
    }

    //--------------------------------------------------------------
    void RectTransform::OnCreate() {
        // Creating a rect transform means replacing the current one.
        Transform *obsolete = GetEntity()->m_transform;

        // We need to copy everything over from the now obsolete transform we are about to destroy.
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

        // Update the entity.
        Entity *entity = GetEntity();
        entity->m_transform = this;
        entity->m_components.erase(Type::get<Transform>());
        entity->m_components[Type::get<RectTransform>()] = this;

        // Update the parent.
        Transform *parent = entity->m_transform->m_parent;
        if (parent) {
            auto begin = parent->m_children.begin();
            auto end = parent->m_children.end();
            auto pos = std::find(begin, end, obsolete);
            if (pos != end) {
                uint64 index = std::distance(begin, pos);
                parent->m_children[index] = this;
            }
        }

        // Update the children.
        for (Transform *child : m_children) {
            child->m_parent = this;
        }

        // We can now delete the obsolete transform component that is no longer being used.
        // NOTE: This is propbably a little unsafe just deleting the component like that but works for now.
        delete obsolete;
    }

    //--------------------------------------------------------------
    void RectTransform::OnDestroy() {
        if (m_replace_on_destroy) {
            Transform *transform = new Transform();

            // We need to copy everything over from the now obsolete transform we are about to destroy.
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

            // Update the entity.
            Entity *entity = GetEntity();
            transform->m_entity = entity;
            entity->m_transform = transform;
            entity->m_components.erase(Type::get<RectTransform>());
            entity->m_components[Type::get<Transform>()] = transform;

            // Update the children.
            for (Transform *child : m_children) {
                child->m_parent = transform;
            }
        } else {
            Transform::OnDestroy();
        }
    }

    //--------------------------------------------------------------
    RectTransform *RectTransform::Create() {
        return new RectTransform();
    }

    //--------------------------------------------------------------
    bool RectTransformUtility::RectangleContainsScreenPoint(RectTransform *rect_transform, Vec2 screen_point) {
        // First we need to transform the screen point so that the origin is in the center
        float32 display_half_width = static_cast<float32>(Display::GetWidth()) / 2.0f;
        float32 display_half_height = static_cast<float32>(Display::GetHeight()) / 2.0f;
        screen_point = Vec2(screen_point.x - display_half_width, screen_point.y - display_half_height);

        Vec3 world_corners[4];
        rect_transform->GetWorldCorners(world_corners);

        Vec2 p1 = world_corners[0];
        Vec2 p2 = world_corners[1];
        Vec2 p3 = world_corners[2];
        Vec2 p4 = world_corners[3];

        // NOTE: Counter clockwise order of points is important
        return (IsLeft(p1, p4, screen_point) > 0 && IsLeft(p4, p3, screen_point) > 0 && IsLeft(p3, p2, screen_point) > 0 && IsLeft(p2, p1, screen_point) > 0);
    }

    //--------------------------------------------------------------
    float32 RectTransformUtility::IsLeft(Vec2 p0, Vec2 p1, Vec2 p2) {
        return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    }

}