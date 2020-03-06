#pragma once

#include "hyperion/entity/components/physics/collider.hpp"

namespace Hyperion {

    class SphereCollider : public Collider {
        HYP_OBJECT(SphereCollider, Collider);
    private:
        f32 m_radius = 0.5f;
    public:
        SphereCollider() : Collider("SphereCollider") {}

        ColliderType GetColliderType() const { return ColliderType::SphereCollider; }

        inline f32 GetRadius() const { return m_radius; }
        inline void SetRadius(f32 radius) {
            m_radius = radius;
            NotifyColliderChange();
        }

        void OnMessage(EntityMessage message) override;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    private:
        void NotifyColliderChange();
    };

}