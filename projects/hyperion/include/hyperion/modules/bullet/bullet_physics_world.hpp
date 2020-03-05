#pragma once

#include "hyperion/physics/physics_world.hpp"

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {

    class BulletPhysicsDriver;

    class BulletPhysicsWorld : public PhysicsWorld {
    private:
        btCollisionWorld *m_collision_world;
    public:
        BulletPhysicsWorld(BulletPhysicsDriver *driver);
        ~BulletPhysicsWorld() override;

        bool Raycast(Ray ray, RaycastResult &result, f32 distance);
    };

}