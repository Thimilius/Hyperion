#pragma once

//---------------------- Library Includes ----------------------
#include <bullet/btBulletCollisionCommon.h>

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Physics {
    class BulletPhysicsWorld;
}

//-------------------- Definition Namespace --------------------
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