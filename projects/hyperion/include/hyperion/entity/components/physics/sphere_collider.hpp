#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/physics/collider.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class SphereCollider final : public Collider {
        HYP_REFLECT(Collider);
    public:
        ColliderType GetColliderType() const { return ColliderType::SphereCollider; }

        inline float32 GetRadius() const { return m_radius; }
        inline void SetRadius(float32 radius) {
            m_radius = radius;
            NotifyColliderChange();
        }

        void OnEntityMessage(EntityMessage message) override;
    protected:
        void OnCreate() override;
    private:
        SphereCollider() : Collider("SphereCollider") {}
    private:
        void NotifyColliderChange();
    private:
        static SphereCollider *Create();
    private:
        float32 m_radius = 0.5f;
    };

}