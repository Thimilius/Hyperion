#pragma once

#include "hyperion/entity/components/physics/collider.hpp"

namespace Hyperion {

    class SphereCollider final : public Collider {
        HYP_REFLECT(Collider);
    public:
        SphereCollider() : Collider("SphereCollider") {}

        ColliderType GetColliderType() const { return ColliderType::SphereCollider; }

        inline float32 GetRadius() const { return m_radius; }
        inline void SetRadius(float32 radius) {
            m_radius = radius;
            NotifyColliderChange();
        }

        void OnMessage(EntityMessage message) override;
    protected:
        void OnCreate() override;
    private:
        void NotifyColliderChange();
    private:
        float32 m_radius = 0.5f;
    };

}