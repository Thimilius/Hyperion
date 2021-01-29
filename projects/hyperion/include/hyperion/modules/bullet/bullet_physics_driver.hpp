#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include "hyperion/physics/physics_driver.hpp"

namespace Hyperion::Physics {
    class BulletPhysicsWorld;
}

namespace Hyperion::Physics {

    class BulletPhysicsDriver : public IPhysicsDriver {
    public:
        PhysicsBackend GetBackend() const override { return PhysicsBackend::Bullet; }

        PhysicsWorld *CreatePhysicsWorld() override;
        void DestroyPhysicsWorld(PhysicsWorld *world) override;

        void Initialize() override;
        void FixedUpdate(float32 delta_time) override;
        void Shutdown() override;
    private:
        btDefaultCollisionConfiguration *m_collision_configuration;
        btCollisionDispatcher *m_collision_dispatcher;
        btBroadphaseInterface *m_collision_broadphase_interface;
    private:
        friend class Hyperion::Physics::BulletPhysicsWorld;
    };

}