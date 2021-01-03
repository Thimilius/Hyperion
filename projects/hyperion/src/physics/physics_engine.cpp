#include "hyppch.hpp"

#include "hyperion/physics/physics_engine.hpp"

#include "hyperion/entity/world_manager.hpp"

#if HYP_PHYSICS_BULLET
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"
#endif

namespace Hyperion::Physics {

    bool PhysicsEngine::Raycast(Ray ray, RaycastResult &result, f32 distance) {
        return WorldManager::GetActiveWorld()->GetPhysicsWorld()->Raycast(ray, result, distance);
    }

    void PhysicsEngine::Init() {
#if HYP_PHYSICS_BULLET
        s_physics_driver = new BulletPhysicsDriver();
#else
        s_physics_driver = new DummyPhysicsDriver();
#endif

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