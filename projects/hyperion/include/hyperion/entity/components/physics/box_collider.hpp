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

        inline Vec3 GetOrigin() const { return m_origin; }
        inline void SetOrigin(Vec3 origin) {
            m_origin = origin;
            NotifyColliderChange();
        }
        inline Vec3 GetSize() const { return m_size; }
        inline void SetSize(Vec3 size) {
            m_size = size;
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