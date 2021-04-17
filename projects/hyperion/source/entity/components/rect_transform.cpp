//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rect_transform.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void RectTransform::SetAnchoringPreset(AnchoringPreset anchoring_preset) {
        switch (anchoring_preset) {
            case AnchoringPreset::TopLeft: {
                m_anchor_min = Vec2(0.0f, 1.0f);
                m_anchor_max = Vec2(0.0f, 1.0f);
                m_pivot = Vec2(0.0f, 1.0f);
                break;
            }
            case AnchoringPreset::TopCenter: {
                m_anchor_min = Vec2(0.5f, 1.0f);
                m_anchor_max = Vec2(0.5f, 1.0f);
                m_pivot = Vec2(0.5f, 1.0f);
                break;
            }
            case AnchoringPreset::TopRight: {
                m_anchor_min = Vec2(1.0f, 1.0f);
                m_anchor_max = Vec2(1.0f, 1.0f);
                m_pivot = Vec2(1.0f, 1.0f);
                break;
            }
            case AnchoringPreset::MiddleLeft: {
                m_anchor_min = Vec2(0.0f, 0.5f);
                m_anchor_max = Vec2(0.0f, 0.5f);
                m_pivot = Vec2(0.0f, 0.5f);
                break;
            }
            case AnchoringPreset::MiddleCenter: {
                m_anchor_min = Vec2(0.5f, 0.5f);
                m_anchor_max = Vec2(0.5f, 0.5f);
                m_pivot = Vec2(0.5f, 0.5f);
                break;
            }
            case AnchoringPreset::MiddleRight: {
                m_anchor_min = Vec2(1.0f, 0.5f);
                m_anchor_max = Vec2(1.0f, 0.5f);
                m_pivot = Vec2(1.0f, 0.5f);
                break;
            }
            case AnchoringPreset::BottomLeft: {
                m_anchor_min = Vec2(0.0f, 0.0f);
                m_anchor_max = Vec2(0.0f, 0.0f);
                m_pivot = Vec2(0.0f, 0.0f);
                break;
            }
            case AnchoringPreset::BottomCenter: {
                m_anchor_min = Vec2(0.5f, 0.0f);
                m_anchor_max = Vec2(0.5f, 0.0f);
                m_pivot = Vec2(0.5f, 0.0f);
                break;
            }
            case AnchoringPreset::BottomRight: {
                m_anchor_min = Vec2(1.0f, 0.0f);
                m_anchor_max = Vec2(1.0f, 0.0f);
                m_pivot = Vec2(1.0f, 0.0f);
                break;
            }
            case AnchoringPreset::TopStretchHorizontal: {
                m_anchor_min = Vec2(0.0f, 1.0f);
                m_anchor_max = Vec2(1.0f, 1.0f);
                m_pivot = Vec2(0.5f, 1.0f);
                m_size.x = 0.0f;
                break;
            }
            case AnchoringPreset::MiddleStretchHorizontal: {
                m_anchor_min = Vec2(0.0f, 0.5f);
                m_anchor_max = Vec2(1.0f, 0.5f);
                m_pivot = Vec2(0.5f, 0.5f);
                m_size.x = 0.0f;
                break;
            }
            case AnchoringPreset::BottomStretchHorizontal: {
                m_anchor_min = Vec2(0.0f, 0.0f);
                m_anchor_max = Vec2(1.0f, 0.0f);
                m_pivot = Vec2(0.5f, 0.0f);
                m_size.x = 0.0f;
                break;
            }
            case AnchoringPreset::LeftStretchVertical: {
                m_anchor_min = Vec2(0.0f, 0.0f);
                m_anchor_max = Vec2(0.0f, 1.0f);
                m_pivot = Vec2(0.0f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case AnchoringPreset::CenterStretchVertical: {
                m_anchor_min = Vec2(0.5f, 0.0f);
                m_anchor_max = Vec2(0.5f, 1.0f);
                m_pivot = Vec2(0.5f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case AnchoringPreset::RightStretchVertical: {
                m_anchor_min = Vec2(1.0f, 0.0f);
                m_anchor_max = Vec2(1.0f, 1.0f);
                m_pivot = Vec2(1.0f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case AnchoringPreset::StretchAll: {
                m_anchor_min = Vec2(0.0f, 0.0f);
                m_anchor_max = Vec2(1.0f, 1.0f);
                m_pivot = Vec2(0.5f, 0.5f);
                m_size = Vec2(0.0f, 0.0f);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        NotifyTransformChanged();
    }

    //--------------------------------------------------------------
    void RectTransform::GetLocalCorners(Vec3 corners[4]) const {
        HYP_ASSERT(corners);

        Vec2 min = m_rect.GetMin();
        Vec2 max = m_rect.GetMax();

        corners[0] = Vec3(max.x, max.y, 0.0f);
        corners[1] = Vec3(max.x, min.y, 0.0f);
        corners[2] = Vec3(min.x, min.y, 0.0f);
        corners[3] = Vec3(min.x, max.y, 0.0f);
    }

    //--------------------------------------------------------------
    void RectTransform::GetWorldCorners(Vec3 corners[4]) const {
        HYP_ASSERT(corners);

        GetLocalCorners(corners);
        for (uint64 i = 0; i < 4; i++) {
            corners[i] = m_local_to_world_matrix * corners[i];
        }
    }

    //--------------------------------------------------------------
    void RectTransform::Serialize(ISerializationStream &stream) {
        Transform::Serialize(stream);

        stream.WriteVec2("size", m_size);
        stream.WriteVec2("pivot", m_pivot);
        stream.WriteVec2("anchor_min", m_anchor_min);
        stream.WriteVec2("anchor_max", m_anchor_max);
        stream.WriteVec3("anchored_position", m_anchored_position);
    }

    //--------------------------------------------------------------
    void RectTransform::Deserialize(IDeserializationStream &stream) {
        Transform::Deserialize(stream);

        m_size = stream.ReadVec2("size");
        m_pivot = stream.ReadVec2("pivot");
        m_anchor_min = stream.ReadVec2("anchor_min");
        m_anchor_max = stream.ReadVec2("anchor_max");
        m_anchored_position = stream.ReadVec3("anchored_position");
    }

    //--------------------------------------------------------------
    void RectTransform::OnCreate() {
        // Creating a rect transform means replacing the current one.
        Transform *obsolete = GetEntity()->m_transform;

        // We do not need to replace ourselves.
        // This case happens on deserialization. In that case just call the base function.
        if (obsolete == this) {
            Transform::OnCreate();
            return;
        }

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
        entity->m_components.erase(Type::Get<Transform>());
        entity->m_components.emplace(Type::Get<RectTransform>(), this);

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
            entity->m_components.erase(Type::Get<RectTransform>());
            entity->m_components.emplace(Type::Get<Transform>(), transform);

            // Update the children.
            for (Transform *child : m_children) {
                child->m_parent = transform;
            }
        } else {
            Transform::OnDestroy();
        }
    }

    //--------------------------------------------------------------
    void RectTransform::RecalculateTransform() {
        Vec2 pivot = m_pivot;
        Vec2 size = m_size;
        Vec2 parent_size;
        Vec2 parent_pivot;
        if (m_parent && m_parent->GetType() == Type::Get<RectTransform>()) {
            RectTransform *parent_rect_transform = static_cast<RectTransform *>(m_parent);
            parent_pivot = parent_rect_transform->m_pivot;
            parent_size = parent_rect_transform->m_rect.size;
        } else {
            parent_pivot = Vec2(0.5f, 0.5f);
            parent_size = Vec2(0.0f, 0.0f);
        }
        Vec2 half_parent_size = parent_size / 2.0f;

        float32 anchor_x = m_anchor_max.x - m_anchor_min.x;
        float32 anchor_y = m_anchor_max.y - m_anchor_min.y;
        float32 anchor_x_size = anchor_x * parent_size.x;
        float32 anchor_y_size = anchor_y * parent_size.y;
        size.x += anchor_x * parent_size.x;
        size.y += anchor_y * parent_size.y;
        m_rect = Rect(-pivot * size, size);

        // NOTE: A rect transform has full control over the local position, so that the anchoring can be set properly.
        // First we have our anchored position which acts as a simple offset.
        m_local_position = m_anchored_position;
        // We have to take into account the parent pivot which has an effect on our local position.
        m_local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
        m_local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
        // Now we need to take into account our own pivot.
        m_local_position.x += m_anchor_min.x * parent_size.x - half_parent_size.x;
        m_local_position.y += m_anchor_min.y * parent_size.y - half_parent_size.y;
        // And finally our anchoring.
        m_local_position.x += (m_pivot.x) * anchor_x_size;
        m_local_position.y += (m_pivot.y) * anchor_y_size;

        Transform::RecalculateTransform();
    }

    //--------------------------------------------------------------
    bool RectTransformUtility::RectangleContainsScreenPoint(RectTransform *rect_transform, Vec2 screen_point) {
        Vec3 world_corners[4];
        rect_transform->GetWorldCorners(world_corners);

        Vec2 p1 = world_corners[0];
        Vec2 p2 = world_corners[1];
        Vec2 p3 = world_corners[2];
        Vec2 p4 = world_corners[3];

        // NOTE: Counter clockwise order of points is important.
        return (IsLeft(p1, p4, screen_point) >= 0.0f && IsLeft(p4, p3, screen_point) >= 0.0f && IsLeft(p3, p2, screen_point) >= 0.0f && IsLeft(p2, p1, screen_point) >= 0.0f);
    }

    //--------------------------------------------------------------
    float32 RectTransformUtility::IsLeft(Vec2 p0, Vec2 p1, Vec2 p2) {
        return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(RectTransform)
    HYP_REFLECT_BASE(Transform)
    HYP_REFLECT_CONSTRUCTOR([]() { return new RectTransform(); })
    HYP_REFLECT_END()

}