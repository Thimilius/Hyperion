#include "hyppch.hpp"

#include "hyperion/physics/physics_engine.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"
#include "hyperion/entity/world_manager.hpp"

namespace Hyperion::Physics {

    bool PhysicsEngine::Raycast(Ray ray, RaycastResult &result, f32 distance) {
        return WorldManager::GetActiveWorld()->GetPhysicsWorld()->Raycast(ray, result, distance);
    }

    void PhysicsEngine::Init(const PhysicsSettings &settings) {
        s_physics_backend = settings.backend;

        switch (settings.backend) {
            case PhysicsBackend::None:
                s_physics_driver = new DummyPhysicsDriver();
                break;
            case PhysicsBackend::Bullet:
                s_physics_driver = new BulletPhysicsDriver();
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return;
        }

        s_physics_driver->Init();
    }

    void PhysicsEngine::Update(f32 delta_time) {
        s_physics_driver->Update(delta_time);
    }

    void PhysicsEngine::Shutdown() {
        s_physics_driver->Shutdown();
        delete s_physics_driver;
    }

}