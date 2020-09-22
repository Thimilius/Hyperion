#pragma once

#include "hyperion/physics/physics_driver.hpp"

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {
    class BulletPhysicsWorld;
}

namespace Hyperion::Physics {

    class BulletPhysicsDriver : public PhysicsDriver {
    public:
        PhysicsWorld *CreatePhysicsWorld() override;
        void DestroyPhysicsWorld(PhysicsWorld *world) override;

        void Init() override;
        void Update(f32 delta_time) override;
        void Shutdown() override;
    private:
        btDefaultCollisionConfiguration *m_collision_configuration;
        btCollisionDispatcher *m_collision_dispatcher;
        btBroadphaseInterface *m_collision_broadphase_interface;
    private:
        friend class Hyperion::Physics::BulletPhysicsWorld;
    };

}