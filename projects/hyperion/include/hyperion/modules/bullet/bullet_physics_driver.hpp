#pragma once

#include "hyperion/physics/physics_driver.hpp"

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {

    class BulletPhysicsDriver : public PhysicsDriver {
    private:
        btDefaultCollisionConfiguration *m_collision_configuration;
        btCollisionDispatcher *m_collision_dispatcher;
        btBroadphaseInterface *m_collision_broadphase_interface;
        btCollisionWorld *m_collision_world;
        
    public:
        void Init() override;
        void Update(f32 delta_time) override;
        void Shutdown() override;
    };

}