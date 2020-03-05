#include "hyppch.hpp"

#include "hyperion/physics/physics_engine.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

namespace Hyperion::Physics {

    void PhysicsEngine::Init(PhysicsBackend backend) {
        switch (backend) {
            case PhysicsBackend::None:
                s_physics_driver.reset(new DummyPhysicsDriver());
                break;
            case PhysicsBackend::Bullet:
                s_physics_driver.reset(new BulletPhysicsDriver());
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return;
        }

        s_physics_backend = backend;
        s_physics_driver->Init();
    }

}