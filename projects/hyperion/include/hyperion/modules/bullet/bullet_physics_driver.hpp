#pragma once

#include "hyperion/physics/physics_driver.hpp"

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {

    class BulletPhysicsDriver : public PhysicsDriver {
    private:
        btDefaultCollisionConfiguration *m_collision_configuration;
        btCollisionDispatcher *m_collision_dispatcher;
        btBroadphaseInterface *m_collision_broadphase_interface;
    public:
        PhysicsWorld *CreatePhysicsWorld() override;
        void DestroyPhysicsWorld(PhysicsWorld *world) override;

        void Init() override;
        void Update(f32 delta_time) override;
        void Shutdown() override;

        friend class BulletPhysicsWorld;
    };

}