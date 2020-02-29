#pragma once

#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/core/math/quaternion.hpp"

namespace Hyperion {

    class Transform : public Component, EntityMessageListener {
        HYP_OBJECT(Transform, Component)
    private:
        Vec3 m_local_position = Vec3::Zero();
        Quaternion m_local_rotation = Quaternion::Identity();
        Vec3 m_local_scale = Vec3::One();

        Vec3 m_derived_position = Vec3::Zero();
        Quaternion m_derived_rotation = Quaternion::Identity();
        Vec3 m_derived_scale = Vec3::One();

        Mat4 m_local_to_world_matrix = Mat4::Identity();
        Mat4 m_world_to_local_matrix = Mat4::Identity();

        Transform *m_parent = nullptr;
        Vector<Transform*> m_children;
    public:
        inline Vec3 GetLocalPosition() const { return m_local_position; }
        inline void SetLocalPosition(const Vec3 &position) {
            m_local_position = position;
            NotifyTransformChange();
        }

        inline Quaternion GetLocalRotation() const { return m_local_rotation; }
        inline void SetLocalRotation(const Quaternion &rotation) {
            m_local_rotation = rotation;
            NotifyTransformChange();
        }
        inline Vec3 GetLocalEulerAngles() const { return m_local_rotation.ToEulerAngles(); }
        inline void SetLocalEulerAngles(const Vec3 &angles) { SetLocalRotation(Quaternion::FromEulerAngles(angles)); }

        inline Vec3 GetLocalScale() const { return m_local_scale; }
        inline void SetLocalScale(const Vec3 &scale) {
            m_local_scale = scale;
            NotifyTransformChange();
        }

        inline Vec3 GetPosition() const { return m_derived_position; }
        inline void SetPosition(const Vec3 position) {
            m_local_position = m_parent ? m_parent->WorldToLocalPosition(position) : position;
            NotifyTransformChange();
        }

        inline Quaternion GetRotation() const { return m_derived_rotation; }
        inline void SetRotation(const Quaternion &rotation) {
             m_local_rotation = m_parent ? m_parent->WorldToLocalRotation(rotation) : rotation;
            NotifyTransformChange();
        }
        inline Vec3 GetEulerAngles() const { return m_derived_rotation.ToEulerAngles(); }
        inline void SetEulerAngles(const Vec3 &angles) { SetRotation(Quaternion::FromEulerAngles(angles)); }

        inline Vec3 GetRight() const {
            return m_derived_rotation * Vec3::Right();
        }
        inline Vec3 GetUp() const {
            return m_derived_rotation * Vec3::Up();
        }
        inline Vec3 GetForward() const {
            return m_derived_rotation * Vec3::Forward();
        }

        inline Vec3 WorldToLocalPosition(const Vec3 &position) const {
            return (m_derived_rotation.Inverted() * (position - m_derived_position)) / m_derived_scale;
        }
        inline Vec3 LocalToWorldPosition(const Vec3 &position) const {
            return (m_derived_rotation * (m_derived_scale * m_local_position)) + m_derived_position;
        }

        inline Quaternion WorldToLocalRotation(const Quaternion &rotation) const {
            return m_derived_rotation.Inverted() * rotation;
        }
        inline Quaternion LocalToWorldRotation(const Quaternion &rotation) const {
            return m_derived_rotation * rotation;
        }

        inline Mat4 GetLocalToWorldMatrix() const { return m_local_to_world_matrix; }
        inline Mat4 GetWorldToLocalMatrix() const { return m_world_to_local_matrix; }

        inline u32 GetChildCount() const { return (u32)m_children.size(); }
        inline Transform *GetChild(u32 index) { return m_children[index]; }
        Transform *GetRoot() const;

        void SetParent(Transform *parent);

        void OnMessage(EntityMessage message) override;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        Transform() : Component("Transform") { }

        void NotifyTransformChange();
        void RecalculateTransform();

        friend class Entity;
    };

}