#pragma once

#include "hyperion/entity/components/entity_component.hpp"
#include "hyperion/core/math/quaternion.hpp"

namespace Hyperion {

    // TODO: Add transform hierarchy
    class TransformComponent : public EntityComponent {
    private:
        Vec3 m_position = Vec3::Zero();
        Quaternion m_rotation = Quaternion::Identity();
        Vec3 m_scale = Vec3::One();

        Mat4 m_local_to_world_matrix = Mat4::Identity();
    public:
        inline Vec3 GetPosition() const { return m_position; }
        inline void SetPosition(const Vec3 &position) { m_position = position; RecalculateLocalToWorldMatrix(); }

        inline Quaternion GetRotation() const { return m_rotation; }
        inline void SetRotation(const Quaternion &rotation) { m_rotation = rotation; RecalculateLocalToWorldMatrix(); }

        inline Vec3 GetScale() const { return m_scale; }
        inline void SetScale(const Vec3 &scale) { m_scale = scale; RecalculateLocalToWorldMatrix(); }
    private:
        TransformComponent() = default;

        inline void RecalculateLocalToWorldMatrix() { m_local_to_world_matrix = Mat4::TRS(m_position, m_rotation, m_scale); }

        friend class Entity;
    };

}