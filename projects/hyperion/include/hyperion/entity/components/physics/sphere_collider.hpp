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

        f32 GetRadius() const { return m_radius; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}