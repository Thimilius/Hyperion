#pragma once

#include "hyperion/entity/entity_event.hpp"
#include "hyperion/entity/components/entity_component.hpp"
#include "hyperion/core/math/quaternion.hpp"

namespace Hyperion {

    // TODO: Properly destinguish between local and world transformation
    class TransformComponent : public EntityComponent, EntityEventListener {
        HYP_OBJECT(TransformComponent, EntityComponent)
    private:
        Vec3 m_position = Vec3::Zero();
        Quaternion m_rotation = Quaternion::Identity();
        Vec3 m_scale = Vec3::One();

        Mat4 m_local_to_world_matrix = Mat4::Identity();
        Mat4 m_world_to_local_matrix = Mat4::Identity();

        TransformComponent *m_parent = nullptr;
        Vector<TransformComponent*> m_children;
    public:
        inline Vec3 GetPosition() const { return m_position; }
        inline void SetPosition(const Vec3 &position) { m_position = position; NotifyTransformChange(); }

        inline Quaternion GetRotation() const { return m_rotation; }
        inline void SetRotation(const Quaternion &rotation) { m_rotation = rotation; NotifyTransformChange(); }

        inline Vec3 GetScale() const { return m_scale; }
        inline void SetScale(const Vec3 &scale) { m_scale = scale; NotifyTransformChange(); }

        inline Mat4 GetLocalToWorldMatrix() const { return m_local_to_world_matrix; }

        inline u32 GetChildCount() const { return (u32)m_children.size(); }
        inline TransformComponent *GetChild(u32 index) { return m_children[index]; }

        void SetParent(TransformComponent *parent);

        void OnEvent(EntityEvent event) override;
    protected:
        virtual ~TransformComponent() override;

        void OnCreate() override;
        void OnDestroy() override;
    private:
        TransformComponent() = default;

        void NotifyTransformChange();
        void RecalculateMatricies();

        friend class Entity;
    };

}