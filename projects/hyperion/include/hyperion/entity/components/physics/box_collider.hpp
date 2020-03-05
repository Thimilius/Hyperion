#pragma once

#include "hyperion/entity/components/physics/collider.hpp"

namespace Hyperion {

    class BoxCollider : public Collider {
        HYP_OBJECT(BoxCollider, Collider);
    private:
        Vec3 m_origin = Vec3::Zero();
        Vec3 m_size = Vec3::One();
    public:
        BoxCollider() : Collider("BoxCollider") { }

        ColliderType GetColliderType() const { return ColliderType::BoxCollider; }

        Vec3 GetOrigin() const { return m_origin; }
        Vec3 GetSize() const { return m_size; }
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}