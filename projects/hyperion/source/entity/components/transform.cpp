//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/transform.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Transform::SetParent(Transform *parent) {
        // Handle special edge cases and for now just silently abandon.
        if (m_parent == parent || parent == this) {
            return;
        }
        // We can not set a child from us as a new parent.
        if (IsChildOf(parent)) {
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

    //--------------------------------------------------------------
    Transform *Transform::GetRoot() const {
        Transform *parent = m_parent ? m_parent : (Transform *)this;
        while (parent->m_parent != nullptr) {
            parent = parent->m_parent;
        }
        return parent;
    }

    //--------------------------------------------------------------
    bool Transform::IsChildOf(Transform *parent) const {
        Transform *p = parent;
        while (p != nullptr) {

            if (p->m_parent == this) {
                return true;
            }
            p = p->m_parent;
        }
        return false;
    }

    //--------------------------------------------------------------
    void Transform::NotifyTransformChange() {
        RecalculateTransform();

        GetEntity()->DispatchMessage({ EntityMessageType::TransformChanged, nullptr });

        for (Transform *child : m_children) {
            child->NotifyTransformChange();
        }
    }

    //--------------------------------------------------------------
    void Transform::RecalculateTransform() {
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

    //--------------------------------------------------------------
    Transform *Hyperion::Transform::Create() {
        return new Transform();
    }

    //--------------------------------------------------------------
    void Hyperion::Transform::OnCreate() {
        Component::OnCreate();

        RecalculateTransform();
    }

    //--------------------------------------------------------------
    void Transform::OnDestroy() {
        Entity *entity = GetEntity();
        if (m_parent == nullptr) {
            entity->GetWorld()->RemoveRootEntity(entity);
        } else {
            auto begin = m_parent->m_children.begin();
            auto end = m_parent->m_children.end();
            if (std::find(begin, end, this) != end) {
                m_parent->m_children.erase(std::remove(begin, end, this));
            }
        }

        Component::OnDestroy();
    }

}
